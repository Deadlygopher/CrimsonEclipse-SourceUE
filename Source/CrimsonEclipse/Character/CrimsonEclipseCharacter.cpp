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
#include "Net/UnrealNetwork.h"
#include "CrimsonEclipse/CrimsonEclipseComponents/CombatComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "CrimsonEclipse/HUD/PlayerHUD.h"

#include "CrimsonEclipse/CrimsonEclipseComponents/XPComponent.h"
#include "CrimsonEclipse/CrimsonEclipseComponents/CharacterLevelComponent.h"



ACrimsonEclipseCharacter::ACrimsonEclipseCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	XPComponent = CreateDefaultSubobject<UXPComponent>("ExpirienceComponent");

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
}

void ACrimsonEclipseCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (XPComponent) 
	{
		XPComponent->OnReceiveExp.AddUObject(this, &ACrimsonEclipseCharacter::SetExpProgressHUD);
		XPComponent->OnLevelUp.AddUObject(this, &ACrimsonEclipseCharacter::SetNewLevelHUD);
		XPComponent->OnLevelUp.AddUObject(this, &ACrimsonEclipseCharacter::SetNewLevelForLvlComp);
	}
	if (XPComponent && LvlComponent)
	{
		XPComponent->SetCurrentLevel(LvlComponent->GetCurrentLevel());
		SetNewLevelHUD(LvlComponent->GetCurrentLevel());
		//SetExpProgressHUD(XPComponent->GetCurre)
	}
}

void ACrimsonEclipseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME_CONDITION(ACrimsonEclipseCharacter, OverlappingWeapon, COND_OwnerOnly);
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

void ACrimsonEclipseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAction("CameraRotation", IE_Pressed, this, &ACrimsonEclipseCharacter::CameraRotateON);
	PlayerInputComponent->BindAction("CameraRotation", IE_Released, this, &ACrimsonEclipseCharacter::CameraRotateOFF);
	PlayerInputComponent->BindAction("Roll", IE_Pressed, this, &ACEBaseCharacter::StartRoll);

	PlayerInputComponent->BindAction("LightAttack", IE_Pressed, this, &ACrimsonEclipseCharacter::RequestLightAttack);
	PlayerInputComponent->BindAction("HeavyAttack", IE_Pressed, this, &ACrimsonEclipseCharacter::RequestHeavyAttack);

	PlayerInputComponent->BindAxis("CameraRotationAxis", this, &ACrimsonEclipseCharacter::ChangeCameraAngle);
	PlayerInputComponent->BindAxis("CameraZoom", this, &ACrimsonEclipseCharacter::CameraZoom);
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

void ACrimsonEclipseCharacter::SetHealthWidgetInfo(float NewHealth, float MaxHealth)
{
	Super::SetHealthWidgetInfo(NewHealth, MaxHealth);
	if (GetController())
	{
		if (Cast<APlayerController>(GetController())->GetHUD())
		{
			auto HUD = Cast<APlayerController>(GetController())->GetHUD();
			if (HUD)
			{
				Cast<APlayerHUD>(HUD)->UpdateHUDHealth(NewHealth, MaxHealth);
			}
		}
	}
}

void ACrimsonEclipseCharacter::SetExpProgressHUD(int32 CurrentLevelMaxExp, int32 CurrentLevelExp)
{
	if (GetController())
	{
		if (Cast<APlayerController>(GetController())->GetHUD())
		{
			auto HUD = Cast<APlayerController>(GetController())->GetHUD();
			if (HUD)
			{
				Cast<APlayerHUD>(HUD)->UpdateHUDExpBar(CurrentLevelMaxExp, CurrentLevelExp);
			}
		}
	}
}

void ACrimsonEclipseCharacter::SetNewLevelHUD(int32 NewLevel)
{
	if (GetController())
	{
		if (Cast<APlayerController>(GetController())->GetHUD())
		{
			auto HUD = Cast<APlayerController>(GetController())->GetHUD();
			if (HUD)
			{
				Cast<APlayerHUD>(HUD)->UpdateHUDLevelText(NewLevel);
			}
		}
	}
}

void ACrimsonEclipseCharacter::SetNewLevelForLvlComp(int32 NewLevel)
{
	if (LvlComponent) LvlComponent->LevelUp();
}

void ACrimsonEclipseCharacter::StartRoll()
{
	RotateToCursorDirecion();
	Super::StartRoll();
}

void ACrimsonEclipseCharacter::RequestLightAttack()
{
	RotateToCursorDirecion();
	Super::RequestLightAttack();
}


/// ROTATE TO CURSOR IMPLEMENTATION ///

void ACrimsonEclipseCharacter::RotateToCursorDirecion()
{
	Super::RotateToCursorDirecion();
	FHitResult HitResult;
	Cast<APlayerController>(GetController())->GetHitResultUnderCursor(ECollisionChannel::ECC_MAX, false, HitResult);
	FVector LastLocation = GetCharacterMovement()->GetLastUpdateLocation();
	FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(LastLocation, HitResult.Location);

	if (HasAuthority())
	{
		Multicast_RotateToCursorDirecion(NewRotation);
	}
	else
	{
		Server_RotateToCursorDirecion(NewRotation);
		Multicast_RotateToCursorDirecion(NewRotation);
	}
}

void ACrimsonEclipseCharacter::Multicast_RotateToCursorDirecion_Implementation(FRotator NewRotation)
{
	GetCharacterMovement()->MoveUpdatedComponent(FVector{ 0 }, FRotator{ 0.f,NewRotation.Yaw, NewRotation.Roll }, true);
}

void ACrimsonEclipseCharacter::Server_RotateToCursorDirecion_Implementation(FRotator NewRotation)
{
	GetCharacterMovement()->MoveUpdatedComponent(FVector{ 0 }, FRotator{ 0.f,NewRotation.Yaw, NewRotation.Roll }, true);
}

/// ROTATE TO CURSOR IMPLEMENTATION ///