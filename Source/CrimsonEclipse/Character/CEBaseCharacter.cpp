// Fill out your copyright notice in the Description page of Project Settings.


#include "CEBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CrimsonEclipse/CrimsonEclipseComponents/CombatComponent.h"
#include "CrimsonEclipse/CrimsonEclipseComponents/HealthComponent.h"
#include "Components/WidgetComponent.h"
#include "InventoryComponent.h"
#include "Item.h"
#include "CrimsonEclipse/HUD/OverheadWidget.h"
#include "CrimsonEclipse/Items/Weapon.h"


ACEBaseCharacter::ACEBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	CombatComponent->SetIsReplicated(true);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	OverheadWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidgetComponent->SetupAttachment(RootComponent);

	GetCharacterMovement()->MaxWalkSpeed = MaxMoveSpeed;

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
	InventoryComponent->OnItemEquipped.AddDynamic(this, &ACEBaseCharacter::OnItemEquip);
	InventoryComponent->OnItemUnequipped.AddDynamic(this, &ACEBaseCharacter::OnItemUnequip);
}

void ACEBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetHealthWidgetInfo(HealthComponent->GetHealth(), HealthComponent->GetMaxHealth());

	HealthComponent->OnHealthChange.AddUObject(this, &ACEBaseCharacter::SetHealthWidgetInfo);
	if (IsPlayerControlled())
	{
		OverheadWidgetComponent->SetVisibility(false);
	}
	OnTakeAnyDamage.AddDynamic(this, &ACEBaseCharacter::ReceiveDamage);
}

void ACEBaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (CombatComponent)
	{
		CombatComponent->SetCharacter(this);
	}
}

void ACEBaseCharacter::LightAttack()
{
	if (CombatComponent->GetRightHandWeapon() && bReadyForAttack)
	{
		//RotateToCursorDirecion();
		GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &ACEBaseCharacter::ResetReadyForAttack);
		auto AnimMontage = CombatComponent->GetRightHandWeapon()->GetLightAttackAnimMontage();
		if (AnimMontage)
		{
			bReadyForAttack = false;
			GetCharacterMovement()->MaxWalkSpeed = InAttackMoveSpeed;
			GetMesh()->GetAnimInstance()->Montage_Play(AnimMontage);
		}
	}
}

void ACEBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACEBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UHealthComponent* ACEBaseCharacter::GetHealthComponent()
{
	return HealthComponent;
}

float ACEBaseCharacter::GetHealth()
{
	return HealthComponent->GetHealth();
}

float ACEBaseCharacter::GetMaxHealth()
{
	return HealthComponent->GetMaxHealth();
}

void ACEBaseCharacter::StartRoll()
{
	bReadyForAttack = false;
	GetCharacterMovement()->MaxWalkSpeed = RollSpeed;
	GetCharacterMovement()->Velocity = GetActorForwardVector() * RollSpeed;
	bPressedRoll = true;
}

void ACEBaseCharacter::RollInProcess()
{
	GetCharacterMovement()->Velocity = GetActorForwardVector() * RollSpeed;
}

void ACEBaseCharacter::StopRoll()
{
	GetCharacterMovement()->MaxWalkSpeed = MaxMoveSpeed;
	bPressedRoll = false;
	bReadyForAttack = true;
}

void ACEBaseCharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatorController, AActor* DamageCauser)
{
	HealthComponent->DecreaseHealth(Damage);
	UE_LOG(LogTemp, Warning, TEXT("DELEGATE"));
}

void ACEBaseCharacter::ResetReadyForAttack(UAnimMontage* Montage, bool bInterrupted)
{
	bReadyForAttack = true;
	GetCharacterMovement()->MaxWalkSpeed = MaxMoveSpeed;
}

void ACEBaseCharacter::SetHealthWidgetInfo(float NewHealth, float MaxHealth)
{
	if (OverheadWidgetComponent)
	{
		auto Widget = Cast<UOverheadWidget>(OverheadWidgetComponent->GetWidget());
		Widget->UpdateHealthBar(NewHealth, MaxHealth);
	}
}

void ACEBaseCharacter::OnItemEquip(UItem* InItem, EEquipmentSlotType Type, int32 InQuantity)
{
	switch (Type)
	{
	case EEquipmentSlotType::PrimaryWeapon:
	{
		if (InItem->GetWeaponType() && CombatComponent)
		{
			FActorSpawnParameters WeaponSpawnParameters;
			CombatComponent->EquipRightWeapon(GetWorld()->SpawnActor<AWeapon>(InItem->GetWeaponType(), WeaponSpawnParameters));
			CombatComponent->SetRightHandDamage(InItem->GetWeaponDamage());
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

void ACEBaseCharacter::OnItemUnequip(UItem* InItem, EEquipmentSlotType Type, int32 InQuantity)
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