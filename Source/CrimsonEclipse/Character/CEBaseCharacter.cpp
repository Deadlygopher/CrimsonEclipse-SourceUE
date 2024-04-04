// Fill out your copyright notice in the Description page of Project Settings.


#include "CEBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CrimsonEclipse/CrimsonEclipseComponents/CombatComponent.h"
#include "CrimsonEclipse/CrimsonEclipseComponents/HealthComponent.h"
#include "Components/WidgetComponent.h"
#include "InventoryComponent.h"
#include "Item.h"
#include "ItemInstance.h"
#include "CrimsonEclipse/HUD/OverheadWidget.h"
#include "CrimsonEclipse/Items/Weapon.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "CrimsonEclipse/CrimsonEclipseComponents/CharacterLevelComponent.h"
#include "CrimsonEclipse/Interfaces/XPComponentInterface.h"
#include "Net/UnrealNetwork.h"
#include "AIController.h" //TODO Delete

#include "Blueprint/AIBlueprintHelperLibrary.h" //TODO Delete

DEFINE_LOG_CATEGORY(LogCEBaseCharacter);

ACEBaseCharacter::ACEBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	//replicate

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	CombatComponent->SetIsReplicated(true);

	CharHealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("CharHealthComponent"));

	OverheadWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidgetComponent->SetupAttachment(RootComponent);
	OverheadWidgetComponent->SetVisibility(true);

	LvlComponent = CreateDefaultSubobject<UCharacterLevelComponent>(TEXT("Character Level"));

	GetCharacterMovement()->MaxWalkSpeed = MaxMoveSpeed;

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
	InventoryComponent->OnItemEquipped.AddDynamic(this, &ACEBaseCharacter::OnItemEquip);
	InventoryComponent->OnItemUnequipped.AddDynamic(this, &ACEBaseCharacter::OnItemUnequip);

	AttackReachRadius = CreateDefaultSubobject<USphereComponent>("AttackReachRadius");
	AttackReachRadius->SetupAttachment(RootComponent);
	AttackReachRadius->InitSphereRadius(AttackRadius - AttackRadius*0.2f); //TODO Magic Number
}

void ACEBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	LvlComponent->UpdateAllStats();
	SetHealthWidgetInfo(CharHealthComponent->GetHealth(), CharHealthComponent->GetMaxHealth());
	SetLevelWidgetInfo();

	/*if (GetController())
	{
		OverheadWidgetComponent->SetVisibility(false);
		if (GetController()->IsA<AAIController>())
		{
			OverheadWidgetComponent->SetVisibility(true);
		}
	}*/

	OnTakeAnyDamage.AddDynamic(this, &ACEBaseCharacter::ReceiveDamage);
	if(GetMesh()->GetAnimInstance())
	GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &ACEBaseCharacter::ResetReadyForAttack);

	//GetMesh()->GetAnimInstance()->OnMontageStarted.
	if (CharHealthComponent)
	{
		CharHealthComponent->OnHealthChange.AddUObject(this, &ACEBaseCharacter::SetHealthWidgetInfo);
		CharHealthComponent->OnDeath.AddUObject(this, &ACEBaseCharacter::OnDeath);
	}
	AttackReachRadius->SetGenerateOverlapEvents(true);
	AttackReachRadius->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttackReachRadius->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	AttackReachRadius->OnComponentBeginOverlap.AddDynamic(this, &ACEBaseCharacter::OnReachAttackRadius);

}

void ACEBaseCharacter::OnDeath(AActor* DamageCauser)
{
	GetController()->UnPossess();
	Multicast_OnDeath(DamageCauser);

	/*
	GetController()->UnPossess();
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	GetCharacterMovement()->StopActiveMovement();
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode, false);
	GetMesh()->PlayAnimation(DeathAnimation, false);

	if (DamageCauser)
	{
		auto FoundInterface = DamageCauser->FindComponentByInterface<UXPComponentInterface>();
		auto FoundComponent = Cast<IXPComponentInterface>(FoundInterface);
		if (FoundComponent)FoundComponent->ReceiveExp(LvlComponent->GetCurrentExpForKill());
	}

	FTimerHandle DeathAnimationTimer;
	GetWorldTimerManager().SetTimer(DeathAnimationTimer, this, &ACEBaseCharacter::AfterDeathAnimation, 0.5f, false); //TODO MAgic Number
*/
}

void ACEBaseCharacter::Server_OnDeath_Implementation(AActor* DamageCauser)
{
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	GetCharacterMovement()->StopActiveMovement();
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode, false);
	GetMesh()->PlayAnimation(DeathAnimation, false);

	if (DamageCauser)
	{
		auto FoundInterface = DamageCauser->FindComponentByInterface<UXPComponentInterface>();
		auto FoundComponent = Cast<IXPComponentInterface>(FoundInterface);
		if (FoundComponent)FoundComponent->ReceiveExp(LvlComponent->GetCurrentExpForKill());
	}

	FTimerHandle DeathAnimationTimer;
	GetWorldTimerManager().SetTimer(DeathAnimationTimer, this, &ACEBaseCharacter::AfterDeathAnimation, 0.5f, false);
}

void ACEBaseCharacter::Multicast_OnDeath_Implementation(AActor* DamageCauser)
{
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	GetCharacterMovement()->StopActiveMovement();
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode, false);
	GetMesh()->PlayAnimation(DeathAnimation, false);

	if (DamageCauser)
	{
		auto FoundInterface = DamageCauser->FindComponentByInterface<UXPComponentInterface>();
		auto FoundComponent = Cast<IXPComponentInterface>(FoundInterface);
		if (FoundComponent)FoundComponent->ReceiveExp(LvlComponent->GetCurrentExpForKill());
	}

	FTimerHandle DeathAnimationTimer;
	GetWorldTimerManager().SetTimer(DeathAnimationTimer, this, &ACEBaseCharacter::AfterDeathAnimation, 0.5f, false);
}


void ACEBaseCharacter::AfterDeathAnimation()
{
	//UE_LOG(LogCEBaseCharacter, Warning, TEXT("DEAD!!!"))
	OverheadWidgetComponent->SetVisibility(false);
	GetMesh()->SetSimulatePhysics(true);
}


void ACEBaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (CombatComponent)
	{
		CombatComponent->SetCharacter(this);
	}
}


void ACEBaseCharacter::RequestLightAttack()
{
	if (!CombatComponent) return;
	CombatComponent->LightAttack();
}


void ACEBaseCharacter::RequestHeavyAttack()
{
	if (!CombatComponent) return;
	CombatComponent->HeavyAttack();
}


float ACEBaseCharacter::GetMovementDirection() const
{
	const auto VelocityNormal = GetVelocity().GetSafeNormal();
	const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
	const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
	return FMath::RadiansToDegrees(AngleBetween) * FMath::Sign(CrossProduct.Z);
}

void ACEBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACEBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float ACEBaseCharacter::GetMaxWalkSpeed() const
{
	return GetCharacterMovement()->MaxWalkSpeed;
}

void ACEBaseCharacter::SetMaxWalkSpeed(float NewSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}

UHealthComponent* ACEBaseCharacter::GetHealthComponent()
{
	return CharHealthComponent;
}

float ACEBaseCharacter::GetHealth() const
{
	return CharHealthComponent->GetHealth();
}

float ACEBaseCharacter::GetMaxHealth() const
{
	return CharHealthComponent->GetMaxHealth();
}

void ACEBaseCharacter::RotateToCursorDirecion()
{

}

void ACEBaseCharacter::SetAttackRadius(float RadiusForSet)
{
	AttackRadius = RadiusForSet;
	AttackReachRadius->SetSphereRadius(RadiusForSet - RadiusForSet*0.2);  //TODO Magic Number
	//UE_LOG(LogCEBaseCharacter, Warning, TEXT("%f"), AttackReachRadius->GetScaledSphereRadius());
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
	CharHealthComponent->DecreaseHealth(Damage, DamageCauser);

	bIsReceiveHitImpact = true;

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ACEBaseCharacter::IsReceiveHitImpactReset, 0.2f, false); //TODO Magic Number
}


void ACEBaseCharacter::ResetReadyForAttack(UAnimMontage* Montage, bool bInterrupted)
{
	bReadyForAttack = true;
	GetCharacterMovement()->MaxWalkSpeed = MaxMoveSpeed;
}

void ACEBaseCharacter::OnReachAttackRadius(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == TargetActor &&  bAttackClicked)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), GetActorLocation());
		RequestLightAttack();
		bAttackClicked = false;
	}
}


void ACEBaseCharacter::OnClickAttack()
{
	bAttackClicked = true;
	//UE_LOG(LogCEBaseCharacter, Warning, TEXT("%s"), *FString::SanitizeFloat(GetDistanceTo(TargetActor)));
	if (GetDistanceTo(TargetActor) <= AttackRadius)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), GetActorLocation());
		RequestLightAttack();
	}
}

void ACEBaseCharacter::IsReceiveHitImpactReset()
{
	//UE_LOG(LogCEBaseCharacter, Warning, TEXT("RESET IMPACT"))
	bIsReceiveHitImpact = false;
	//GetWorldTimerManager().ClearAllTimersForObject(this);
}

void ACEBaseCharacter::SetHealthWidgetInfo(float NewHealth, float MaxHealth)
{
	Multicast_SetHealthWidgetInfo(NewHealth, MaxHealth);
}

void ACEBaseCharacter::Multicast_SetHealthWidgetInfo_Implementation(float NewHealth, float MaxHealth)
{
	if (OverheadWidgetComponent)
	{
		auto Widget = Cast<UOverheadWidget>(OverheadWidgetComponent->GetWidget());
		if (!Widget) return;
		Widget->UpdateHealthBar(NewHealth, MaxHealth);
	}
}

void ACEBaseCharacter::SetLevelWidgetInfo()
{
	if (OverheadWidgetComponent)
	{
		auto Widget = Cast<UOverheadWidget>(OverheadWidgetComponent->GetWidget());
		if (Widget && LvlComponent)
		{
			Widget->SetOverheadWidgetLevel(LvlComponent->GetCurrentLevel());
		}
	}
}

void ACEBaseCharacter::SetOverheadWidgetVisibility(bool bVisibility)
{
	OverheadWidgetComponent->SetVisibility(bVisibility);
}

void ACEBaseCharacter::OnItemEquip(UItem* InItem, UItemInstance* InIntemInstance, EEquipmentSlotType Type, int32 InQuantity)
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
			CombatComponent->SetRightHandDamage(InIntemInstance->ItemInstanceDamage);
			if (CombatComponent->GetRightHandWeapon())
			{
				if (CombatComponent->GetRightHandWeapon()->GetWeaponType() == EWeaponType::EWT_Range || 
					CombatComponent->GetRightHandWeapon()->GetWeaponType() == EWeaponType::EWT_Magic)
				{
					SetIsRangeWeapon(true);
				}
			}
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
			CombatComponent->SetLeftHandDamage(InIntemInstance->ItemInstanceDamage);
			if (CombatComponent->GetLeftHandWeapon())
			{
				if (CombatComponent->GetLeftHandWeapon()->GetWeaponType() == EWeaponType::EWT_Range ||
					CombatComponent->GetLeftHandWeapon()->GetWeaponType() == EWeaponType::EWT_Magic)
				{
					SetIsRangeWeapon(true);
				}
			}
		}
		break;
	}
	default:
	{
		//UE_LOG(LogCEBaseCharacter, Warning, TEXT("Default EQUIP case"));
		break;
	}
	}
}

void ACEBaseCharacter::OnItemUnequip(UItem* InItem, UItemInstance* InIntemInstance, EEquipmentSlotType Type, int32 InQuantity)
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
		//UE_LOG(LogCEBaseCharacter, Warning, TEXT("Default UNEQUIP case"));
		break;
	}
	}
}


void ACEBaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ACEBaseCharacter, bPressedRoll, COND_None);
	DOREPLIFETIME_CONDITION(ACEBaseCharacter, RollSpeed, COND_None);
	DOREPLIFETIME_CONDITION(ACEBaseCharacter, bIsReceiveHitImpact, COND_None);
}