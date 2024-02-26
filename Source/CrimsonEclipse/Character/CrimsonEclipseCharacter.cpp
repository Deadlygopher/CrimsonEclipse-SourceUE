// Copyright Epic Games, Inc. All Rights Reserved.

#include "CrimsonEclipseCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "CrimsonEclipse/Items/Weapon.h"
#include "CrimsonEclipse/CrimsonEclipseComponents/CombatComponent.h"
#include "InventoryComponent.h"
#include "Item.h"
#include "Kismet/KismetSystemLibrary.h"



ACrimsonEclipseCharacter::ACrimsonEclipseCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(RootComponent);

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	CombatComponent->SetIsReplicated(true);

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent1");
	//InventoryComponent->OnItemEquipped.AddDynamic(this, &ACrimsonEclipseCharacter::OnItemEquip);
	InventoryComponent->OnItemEquipped.AddDynamic(this, &ACrimsonEclipseCharacter::EquipWeapon);
	InventoryComponent->OnItemUnequipped.AddDynamic(this, &ACrimsonEclipseCharacter::OnItemUnequip);
}

void ACrimsonEclipseCharacter::BeginPlay()
{
	Super::BeginPlay();
	//InventoryComponent->OnItemEquipped.AddDynamic(this, &ACrimsonEclipseCharacter::OnItemEquip);
}

void ACrimsonEclipseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ACrimsonEclipseCharacter, OverlappingWeapon, COND_OwnerOnly);
}

void ACrimsonEclipseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (CombatComponent)
	{
		CombatComponent->Character = this;
	}
}

void ACrimsonEclipseCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	if (CursorToWorld != nullptr)
	{
		/*else*/ if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
	}
}



void ACrimsonEclipseCharacter::OnRep_OverlappingWeapon(AWeapon* LastWeapon)
{
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(true);
	}
	if (LastWeapon)
	{
		LastWeapon->ShowPickupWidget(false);
	}
}

void ACrimsonEclipseCharacter::SetOverlappingWeapon(AWeapon* Weapon)
{
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(false);
	}

	OverlappingWeapon = Weapon;
	if (IsLocallyControlled())
	{
		if (OverlappingWeapon)
		{
			OverlappingWeapon->ShowPickupWidget(true);
		}
	}
}

void ACrimsonEclipseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("CameraRotation", IE_Pressed, this, &ACrimsonEclipseCharacter::CameraRotateON);
	PlayerInputComponent->BindAction("CameraRotation", IE_Released, this, &ACrimsonEclipseCharacter::CameraRotateOFF);

	PlayerInputComponent->BindAxis("CameraRotationAxis", this, &ACrimsonEclipseCharacter::ChangeCameraAngle);
	PlayerInputComponent->BindAxis("CameraZoom", this, &ACrimsonEclipseCharacter::CameraZoom);

	//PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &ACrimsonEclipseCharacter::EquipButtonPressed);
}

void ACrimsonEclipseCharacter::CameraRotateON()
{
	bIsCameraRotate = true;
}

void ACrimsonEclipseCharacter::CameraRotateOFF()
{
	bIsCameraRotate = false;
}

void ACrimsonEclipseCharacter::ChangeCameraAngle(float Value)
{
	if (CameraBoom && bIsCameraRotate)
	{
		CameraBoom->AddRelativeRotation(FRotator(0.f, Value*2, 0.f));  // Dont forget get rid of hardcoding
	}
}

void ACrimsonEclipseCharacter::CameraZoom(float Value)
{
	if (CameraBoom)
	{
		float CameraArmLength = CameraBoom->TargetArmLength;
		float NewCameraArmLength = FMath::Clamp(CameraArmLength + Value*30, 400.f, 1600.f);  // Dont forget get rid of hardcoding
		CameraBoom->TargetArmLength = NewCameraArmLength;
	}
}

void ACrimsonEclipseCharacter::EquipWeapon(UItem* InItem, EEquipmentSlotType Type, int32 InQuantity)
{
	//if (CombatComponent)
	//{
		if (HasAuthority())
		{
			UE_LOG(LogTemp, Warning, TEXT("HasAuthority"));
			OnItemEquip(InItem, Type, InQuantity);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("NOTHasAuthority"));
			ServerEquipWeapon(InItem, Type, InQuantity);
		}
	//}
}

void ACrimsonEclipseCharacter::ServerEquipWeapon_Implementation(UItem* InItem, EEquipmentSlotType Type, int32 InQuantity)
{
	//if (CombatComponent)
	//{
		OnItemEquip(InItem, Type, InQuantity);
		UE_LOG(LogTemp, Warning, TEXT("ServerEquip"));
		//CombatComponent->EquipRightWeapon(OverlappingWeapon);
	//}
}

void ACrimsonEclipseCharacter::OnItemEquip(UItem* InItem, EEquipmentSlotType Type, int32 InQuantity)
{
	switch (Type)
	{
	case EEquipmentSlotType::PrimaryWeapon:
	{
		if (InItem->GetWeaponType() && CombatComponent)
		{
			FActorSpawnParameters WeaponSpawnParameters;
			CombatComponent->EquipRightWeapon(GetWorld()->SpawnActor<AWeapon>(InItem->GetWeaponType(), WeaponSpawnParameters));
			UE_LOG(LogTemp, Warning, TEXT("Primary Weapon"));
		}
		break;
	}
	case EEquipmentSlotType::SecondaryWeapon:
	{
		if (InItem->GetWeaponType() && CombatComponent)
		{
			FActorSpawnParameters WeaponSpawnParameters;
			CombatComponent->EquipLeftWeapon(GetWorld()->SpawnActor<AWeapon>(InItem->GetWeaponType(), WeaponSpawnParameters));
			UE_LOG(LogTemp, Warning, TEXT("Secondary Weapon"));
		}
		break;
	}
	default:
	{
		UE_LOG(LogTemp, Warning, TEXT("Default EQUIP case"));
		break;
	}
	}
}

void ACrimsonEclipseCharacter::OnItemUnequip(UItem* InItem, EEquipmentSlotType Type, int32 InQuantity)
{
	switch (Type)
	{
	case EEquipmentSlotType::PrimaryWeapon:
	{
		if (CombatComponent->GetRightHandWeapon())
		{
			CombatComponent->UnequipRightWeapon();
			UE_LOG(LogTemp, Warning, TEXT("Unequip Primary Weapon"));
		}
		break;
	}
	case EEquipmentSlotType::SecondaryWeapon:
	{
		if (CombatComponent->GetLeftHandWeapon())
		{
			CombatComponent->UnequipLeftWeapon();
			UE_LOG(LogTemp, Warning, TEXT("Unequip Secondary Weapon"));
		}
		break;
	}
	default:
	{
		UE_LOG(LogTemp, Warning, TEXT("Default UNEQUIP case"));
		break;
	}
	}
}