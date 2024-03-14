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
#include "Components/SphereComponent.h"

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

	AttackReachRadius = CreateDefaultSubobject<USphereComponent>("AttackReachRadius");
	AttackReachRadius->SetupAttachment(RootComponent);
	AttackReachRadius->SetSphereRadius(100.f, true);
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
	AttackReachRadius->SetGenerateOverlapEvents(false);
	AttackReachRadius->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttackReachRadius->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	AttackReachRadius->OnComponentBeginOverlap.AddDynamic(this, &ACEBaseCharacter::OnReachAttackRadius);
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
			GetCharacterMovement()->MaxWalkSpeed = CombatComponent->GetInAttackMoveSpeed();
			GetMesh()->GetAnimInstance()->Montage_Play(AnimMontage);
		}
	}
	if (CombatComponent->GetLeftHandWeapon() && bReadyForAttack)
	{
		auto AnimMontage = CombatComponent->GetLeftHandWeapon()->GetLightAttackAnimMontage();
		if (AnimMontage)
		{
			bReadyForAttack = false;
			GetCharacterMovement()->MaxWalkSpeed = CombatComponent->GetInAttackMoveSpeed();
			GetMesh()->GetAnimInstance()->Montage_Play(AnimMontage);
		}
	}
}

void ACEBaseCharacter::HeavyAttack()
{
	if (CombatComponent->GetRightHandWeapon() && bReadyForAttack)
	{
		auto AnimMontage = CombatComponent->GetRightHandWeapon()->GetHeavyAttackAnimMontage();
		if (AnimMontage)
		{
			bReadyForAttack = false;
			GetCharacterMovement()->MaxWalkSpeed = CombatComponent->GetInAttackMoveSpeed();
			GetMesh()->GetAnimInstance()->Montage_Play(AnimMontage);
		}
	}
	if (CombatComponent->GetLeftHandWeapon() && bReadyForAttack)
	{
		auto AnimMontage = CombatComponent->GetLeftHandWeapon()->GetHeavyAttackAnimMontage();
		if (AnimMontage)
		{
			bReadyForAttack = false;
			GetCharacterMovement()->MaxWalkSpeed = CombatComponent->GetInAttackMoveSpeed();
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

float ACEBaseCharacter::GetHealth() const
{
	return HealthComponent->GetHealth();
}

float ACEBaseCharacter::GetMaxHealth() const
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
	bIsReceiveHitImpact = true;

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ACEBaseCharacter::IsReceiveHitImpactReset, 0.2f, false);
}

void ACEBaseCharacter::ResetReadyForAttack(UAnimMontage* Montage, bool bInterrupted)
{
	bReadyForAttack = true;
	GetCharacterMovement()->MaxWalkSpeed = MaxMoveSpeed;
}

void ACEBaseCharacter::OnReachAttackRadius(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor == TargetActor)//(OtherActor != this)
	{
		UE_LOG(LogCEBaseCharacter, Warning, TEXT("Activate Overlap %s"), *OtherActor->GetName());
		LightAttack();
		//AttackReachRadius->Deactivate();
		AttackReachRadius->SetGenerateOverlapEvents(false);
		//AttackReachRadius->SetCollisionResponseToAllChannels(ECR_Ignore);
		//AttackReachRadius->OnComponentBeginOverlap.Clear();
	}
}

void ACEBaseCharacter::OnClickAttack()
{
	//AttackReachRadius->Activate();
	AttackReachRadius->SetGenerateOverlapEvents(true);
	UE_LOG(LogCEBaseCharacter, Warning, TEXT("Activate Click"))
}

void ACEBaseCharacter::IsReceiveHitImpactReset()
{
	bIsReceiveHitImpact = false;
	GetWorldTimerManager().ClearAllTimersForObject(this);
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
			checkf(InItem->GetWeaponType()->IsChildOf(AWeapon::StaticClass()), TEXT("Wrong Weapon Class in ItemAsset"));
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
			checkf(InItem->GetWeaponType()->IsChildOf(AWeapon::StaticClass()), TEXT("Wrong Weapon Class in ItemAsset"));
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
