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

DEFINE_LOG_CATEGORY(LogCEBaseCharacter);

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

	if (IsPlayerControlled())
	{
		OverheadWidgetComponent->SetVisibility(false);
	}
	OnTakeAnyDamage.AddDynamic(this, &ACEBaseCharacter::ReceiveDamage);
	GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &ACEBaseCharacter::ResetReadyForAttack);
	if (HealthComponent)
	{
		HealthComponent->OnHealthChange.AddUObject(this, &ACEBaseCharacter::SetHealthWidgetInfo);
	}
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
		auto AnimMontage = CombatComponent->GetRightHandWeapon()->GetLightAttackAnimMontage();
		if (AnimMontage)
		{
			bReadyForAttack = false;
			GetCharacterMovement()->MaxWalkSpeed = CombatComponent->GetRightHandWeapon()->GetWeaponAttackMoveSpeed();
			GetMesh()->GetAnimInstance()->Montage_Play(AnimMontage);
		}
	}
	if (CombatComponent->GetLeftHandWeapon() && bReadyForAttack)
	{
		auto AnimMontage = CombatComponent->GetLeftHandWeapon()->GetLightAttackAnimMontage();
		if (AnimMontage)
		{
			bReadyForAttack = false;
			GetCharacterMovement()->MaxWalkSpeed = CombatComponent->GetLeftHandWeapon()->GetWeaponAttackMoveSpeed();
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

float ACEBaseCharacter::GetMaxWalkSpeed()
{
	return GetCharacterMovement()->MaxWalkSpeed;
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

void ACEBaseCharacter::RotateToCursorDirecion()
{

}

void ACEBaseCharacter::StartRoll()
{
	if (bReadyForAttack != false)
	{
		bReadyForAttack = false;
		GetCharacterMovement()->MaxWalkSpeed = RollSpeed;
		GetCharacterMovement()->Velocity = GetActorForwardVector() * RollSpeed;
		bPressedRoll = true;
	}
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

		if (!Widget) return;
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
			//checkf(InItem->GetWeaponType() != TSubclassOf<AWeapon>(), TEXT("Wrong Weapon class in ItemAsset")) return;
			CombatComponent->EquipRightWeapon(GetWorld()->SpawnActor<AWeapon>(InItem->GetWeaponType(), WeaponSpawnParameters));
			CombatComponent->SetRightHandDamage(InItem->GetWeaponDamage());
		}
		break;
	}
	case EEquipmentSlotType::SecondaryWeapon:
	{
		if (InItem->GetWeaponType() && CombatComponent)
		{
			FActorSpawnParameters WeaponSpawnParameters;
			//checkf(InItem->GetWeaponType() != TSubclassOf<AWeapon>(), TEXT("Wrong Weapon class in ItemAsset")) return;
			CombatComponent->EquipLeftWeapon(GetWorld()->SpawnActor<AWeapon>(InItem->GetWeaponType(), WeaponSpawnParameters));
		}
		break;
	}
	default:
	{
		UE_LOG(LogCEBaseCharacter, Warning, TEXT("Default EQUIP case"));
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
		}
		break;
	}
	case EEquipmentSlotType::SecondaryWeapon:
	{
		if (CombatComponent->GetLeftHandWeapon())
		{
			CombatComponent->UnequipLeftWeapon();
		}
		break;
	}
	default:
	{
		UE_LOG(LogCEBaseCharacter, Warning, TEXT("Default UNEQUIP case"));
		break;
	}
	}
}