// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "CrimsonEclipse/Items/Weapon.h"
#include "CrimsonEclipse/Character/CEBaseCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
//#include "CrimsonEclipse/Projectile/CEProjectileActor.h"
#include "CrimsonEclipse/AI/CEAIController.h"/// TODO DELETE

#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY(LogCombatComponent);

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	//SetIsReplicated(true);
	//ReplicateSubobjects();
}


void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UCombatComponent, ProjectileVector, COND_None);
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

AWeapon* UCombatComponent::GetRightHandWeapon()
{
	return RightHandEquippedWeapon;
}

AWeapon* UCombatComponent::GetLeftHandWeapon()
{
	return LeftHandEquippedWeapon;
}

void UCombatComponent::SetCharacter(ACEBaseCharacter* Char)
{
	Character = Char;
	DefaultInAttackMoveSpeed = Character->GetMaxWalkSpeed();
	InAttackMoveSpeed = DefaultInAttackMoveSpeed;
}

void UCombatComponent::EquipRightWeapon(AWeapon* WeaponToEquip)
{
	if (Character == nullptr || WeaponToEquip == nullptr) return;

	RightHandEquippedWeapon = WeaponToEquip;

	if (RightHandEquippedWeapon) {
		//RightHandEquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);

		const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(FName("RightHandSocket"));
		if (HandSocket)
		{
			HandSocket->AttachActor(RightHandEquippedWeapon, Character->GetMesh());
		}
		RightHandEquippedWeapon->SetOwner(Character);
		RightHandEquippedWeapon->SetActorLocationAndRotation(RightHandEquippedWeapon->GetWeaponMesh()->GetSocketLocation("HandleSocket"),
			RightHandEquippedWeapon->GetWeaponMesh()->GetSocketRotation("HandleSocket"));
		Character->SetAttackRadius(RightHandEquippedWeapon->GetReachRadius());
		if (InAttackMoveSpeed > RightHandEquippedWeapon->GetWeaponAttackMoveSpeed())
		{
			InAttackMoveSpeed = RightHandEquippedWeapon->GetWeaponAttackMoveSpeed();
		}
	}
}

void UCombatComponent::EquipLeftWeapon(AWeapon* WeaponToEquip)
{
	if (Character == nullptr || WeaponToEquip == nullptr) return;

	LeftHandEquippedWeapon = WeaponToEquip;

	if (LeftHandEquippedWeapon)
	{
		//LeftHandEquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);

		const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(FName("LeftHandSocket"));
		if (HandSocket)
		{
			HandSocket->AttachActor(LeftHandEquippedWeapon, Character->GetMesh());
		}
		LeftHandEquippedWeapon->SetOwner(Character);
		LeftHandEquippedWeapon->SetActorLocationAndRotation(LeftHandEquippedWeapon->GetWeaponMesh()->GetSocketLocation("HandleSocket"),
			LeftHandEquippedWeapon->GetWeaponMesh()->GetSocketRotation("HandleSocket"));
		Character->SetAttackRadius(LeftHandEquippedWeapon->GetReachRadius());
		if (InAttackMoveSpeed > LeftHandEquippedWeapon->GetWeaponAttackMoveSpeed())
		{
			InAttackMoveSpeed = LeftHandEquippedWeapon->GetWeaponAttackMoveSpeed();
		}
	}
}

void UCombatComponent::ResetTracingVectors()
{
	if (!RightHandEquippedWeapon) { return; }
	PrevStartSocketLocation = CurrentStartSocketLocation = RightHandEquippedWeapon->GetWeaponMesh()->GetSocketLocation("StartTrackingSocket");
	PrevMiddleSocketLocation = CurrentMiddleSocketLocation = RightHandEquippedWeapon->GetWeaponMesh()->GetSocketLocation("MiddleTrackingSocket");
	PrevEndSocketLocation = CurrentEndSocketLocation = RightHandEquippedWeapon->GetWeaponMesh()->GetSocketLocation("EndTrackingSocket");
	ActorsToIgnore.Empty();
	ActorsToIgnore.Add(Character);
}

void UCombatComponent::WeaponMakeSound()
{
	if (RightHandEquippedWeapon)
	{
		RightHandEquippedWeapon->MakeLightAttackSound();
		RightHandEquippedWeapon->SpawnWeaponTrail();
	}
}

void UCombatComponent::OnHitDetect()
{
	if (RightHandEquippedWeapon)
	{
		const FVector StartSocketLocation = RightHandEquippedWeapon->GetWeaponMesh()->GetSocketLocation("StartTrackingSocket");
		const FVector MiddleSocketLocation = RightHandEquippedWeapon->GetWeaponMesh()->GetSocketLocation("MiddleTrackingSocket");
		const FVector EndSocketLocation = RightHandEquippedWeapon->GetWeaponMesh()->GetSocketLocation("EndTrackingSocket");

		TArray<TEnumAsByte<EObjectTypeQuery>> QueryArray;
		QueryArray.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

		FHitResult HitResult;
		TArray<FHitResult> MultiHitResult;

		if (!GetWorld()) return;

		CurrentStartSocketLocation = StartSocketLocation;
		CurrentMiddleSocketLocation = MiddleSocketLocation;
		CurrentEndSocketLocation = EndSocketLocation;

		UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), PrevStartSocketLocation, CurrentStartSocketLocation,
			QueryArray, false, ActorsToIgnore, EDrawDebugTrace::None,
			HitResult, true /*, FLinearColor::Red, FLinearColor::Green, 2.f*/);

		UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), PrevMiddleSocketLocation, CurrentMiddleSocketLocation,
			QueryArray, false, ActorsToIgnore, EDrawDebugTrace::None,
			HitResult, true /*, FLinearColor::Red, FLinearColor::Green, 2.f*/);

		UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), PrevEndSocketLocation, CurrentEndSocketLocation,
			QueryArray, false, ActorsToIgnore, EDrawDebugTrace::None,
			HitResult, true /*, FLinearColor::Red, FLinearColor::Green, 2.f*/);

		UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), PrevEndSocketLocation, CurrentStartSocketLocation,
			QueryArray, false, ActorsToIgnore, EDrawDebugTrace::None,
			HitResult, true/*, FLinearColor::Red, FLinearColor::Green, 2.f */);

		UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), CurrentStartSocketLocation, CurrentEndSocketLocation, 
			20, QueryArray, false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true/*,
			FLinearColor::Red, FLinearColor::Green, 2.f*/); //TODO MAGIC NUMBER

		PrevStartSocketLocation = CurrentStartSocketLocation;
		PrevMiddleSocketLocation = CurrentMiddleSocketLocation;
		PrevEndSocketLocation = CurrentEndSocketLocation;

		if (HitResult.bBlockingHit)
		{
			Server_ApplyDamageToActor(HitResult);
			Client_ApplyDamageToActor(HitResult);
			/*
			UGameplayStatics::ApplyDamage(HitResult.GetActor(), RightHandDamage,
				Cast<APawn>(GetOwner())->GetController(), GetOwner(), UDamageType::StaticClass());
			ActorsToIgnore.Add(HitResult.GetActor());*/
		}
	}
}

void UCombatComponent::Server_ApplyDamageToActor_Implementation(const FHitResult &HitResult)
{
	UGameplayStatics::ApplyDamage(HitResult.GetActor(), RightHandDamage,
		Cast<APawn>(GetOwner())->GetController(), GetOwner(), UDamageType::StaticClass());
	ActorsToIgnore.Add(HitResult.GetActor());
}

void UCombatComponent::Client_ApplyDamageToActor_Implementation(const FHitResult& HitResult)
{
	ActorsToIgnore.Add(HitResult.GetActor());
}


// ON AIMING IMPLEMENTATION //
void UCombatComponent::OnAiming()
{
	if (LeftHandEquippedWeapon)
	{
		if (Character->GetController() && Character->IsPlayerControlled())
		{
			//Character->RotateToCursorDirecion();
			//Server_OnAiming();
			Client_OnAiming();
		}
	}
}

void UCombatComponent::Client_OnAiming_Implementation()
{
	Character->RotateToCursorDirecion();
}

void UCombatComponent::Server_OnAiming_Implementation()
{
	Character->RotateToCursorDirecion();
}
// ON AIMING IMPLEMENTATION //


// ON PROJECTILE SPAWN REPLICATION //
void UCombatComponent::OnProjectileSpawn()
{
	if (!Character->GetController()) return;
	if (!Cast<ACEAIController>(Character->GetController()))
	{
		if (!Character->HasAuthority())
		{
			Client_ProjectileVector();
			Server_OnProjectileSpawn(ProjectileVector);
		}
		if (Character->HasAuthority() && Character->GetController()->IsLocalPlayerController())
		{
			Client_ProjectileVector();
			Server_OnProjectileSpawn(ProjectileVector);
		}
	}
	else
	{
		const FVector ArrowDirection = Character->GetActorForwardVector();
		LeftHandEquippedWeapon->SpawnProjectile(Character, ArrowDirection, LeftHandDamage);
	}

	/*
	if (GetOwner()->HasAuthority())
	{
		if (LeftHandEquippedWeapon)
		{
			if (Character->IsPlayerControlled())
			{
				FHitResult HitResult;
				Cast<APlayerController>(Character->GetController())->GetHitResultUnderCursor(ECollisionChannel::ECC_MAX, false, HitResult);
				
				FVector ArrowDirection = Character->GetActorForwardVector();
				FTransform SocketTransform = LeftHandEquippedWeapon->GetWeaponMesh()->GetSocketTransform("ArrowSocket");

				FVector ProjectileVector{ HitResult.Location.X - SocketTransform.GetLocation().X,
					HitResult.Location.Y - SocketTransform.GetLocation().Y, ArrowDirection.Z };
				UE_LOG(LogTemp, Warning, TEXT("%f , %f"), ProjectileVector.X, ProjectileVector.Y);

				LeftHandEquippedWeapon->SpawnProjectile(Character, ProjectileVector, LeftHandDamage); //ArrowDirection);
			}
			else
			{
				const FVector ArrowDirection = Character->GetActorForwardVector();
				LeftHandEquippedWeapon->SpawnProjectile(Character, ArrowDirection, LeftHandDamage);
			}
		}
	}
	*/
}

void UCombatComponent::MC_OnProjectileSpawn(FVector VectorToSpawn)
{
	if (LeftHandEquippedWeapon)
	{
		LeftHandEquippedWeapon->SpawnProjectile(Character, VectorToSpawn, LeftHandDamage);
	}
}

void UCombatComponent::Server_OnProjectileSpawn_Implementation(FVector VectorToSpawn)
{
	if (Character->GetController())
	{
		if (LeftHandEquippedWeapon)
		{
			LeftHandEquippedWeapon->SpawnProjectile(Character, VectorToSpawn, LeftHandDamage);
		}
	}
}

void UCombatComponent::Client_ProjectileVector_Implementation()
{
	if (Character->GetController())
	{
		if (LeftHandEquippedWeapon)
		{
			FHitResult HitResult;
			Cast<APlayerController>(Character->GetController())->GetHitResultUnderCursor(ECollisionChannel::ECC_MAX, false, HitResult);

			FVector ArrowDirection = Character->GetActorForwardVector();
			FTransform SocketTransform = LeftHandEquippedWeapon->GetWeaponMesh()->GetSocketTransform("ProjectileSocket");

			ProjectileVector = FVector{ HitResult.Location.X - SocketTransform.GetLocation().X,
				HitResult.Location.Y - SocketTransform.GetLocation().Y, ArrowDirection.Z };
		}
	}
}
// ON PROJECTILE SPAWN REPLICATION //


void UCombatComponent::LightAttack()
{
	if (Character->GetReadyForAttack() && RightHandEquippedWeapon)
	{
		auto const AnimMontage = RightHandEquippedWeapon->GetLightAttackAnimMontage();
		if (!AnimMontage) return;
		Character->SetReadyForAttack(false);
		Character->SetMaxWalkSpeed(InAttackMoveSpeed);
		//Character->PlayAnimMontage(AnimMontage);
		if (GetOwner()->HasAuthority())
		{
			Multicast_PlayAttackMontage(AnimMontage);
		}
		else
		{
			Server_PlayAttackMontage(AnimMontage);
		}
	}
	if (Character->GetReadyForAttack() && LeftHandEquippedWeapon)
	{
		auto const AnimMontage = LeftHandEquippedWeapon->GetLightAttackAnimMontage();
		if (!AnimMontage) return;
		Character->SetReadyForAttack(false);
		Character->SetMaxWalkSpeed(InAttackMoveSpeed);
		//Character->PlayAnimMontage(AnimMontage);
		if (GetOwner()->HasAuthority())
		{
			Multicast_PlayAttackMontage(AnimMontage);
		}
		else
		{
			Server_PlayAttackMontage(AnimMontage);
		}
	}
}

void UCombatComponent::HeavyAttack()
{
	if (Character->GetReadyForAttack() && RightHandEquippedWeapon)
	{
		auto const AnimMontage = RightHandEquippedWeapon->GetHeavyAttackAnimMontage();
		if (!AnimMontage) return;
		Character->SetReadyForAttack(false);
		Character->SetMaxWalkSpeed(InAttackMoveSpeed);
		//Character->PlayAnimMontage(AnimMontage);
		if (GetOwner()->HasAuthority())
		{
			Multicast_PlayAttackMontage(AnimMontage);
		}
		else
		{
			Server_PlayAttackMontage(AnimMontage);
		}
	}
	if (Character->GetReadyForAttack() && LeftHandEquippedWeapon)
	{
		auto const AnimMontage = LeftHandEquippedWeapon->GetHeavyAttackAnimMontage();
		if (!AnimMontage) return;
		Character->SetReadyForAttack(false);
		Character->SetMaxWalkSpeed(InAttackMoveSpeed);
		//Character->PlayAnimMontage(AnimMontage);
		if (GetOwner()->HasAuthority())
		{
			Multicast_PlayAttackMontage(AnimMontage);
		}
		else
		{
			Server_PlayAttackMontage(AnimMontage);
		}
	}
}

void UCombatComponent::Multicast_PlayAttackMontage_Implementation(UAnimMontage* AnimMontage)
{
	if(Character) Character->PlayAnimMontage(AnimMontage);
}
void UCombatComponent::Server_PlayAttackMontage_Implementation(UAnimMontage* AnimMontage)
{
	if (Character) Character->PlayAnimMontage(AnimMontage);
	Multicast_PlayAttackMontage(AnimMontage);
}


void UCombatComponent::UnequipRightWeapon()
{
	if (RightHandEquippedWeapon)
	{
		//const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(FName("RightHandSocket"));
		RightHandEquippedWeapon->Destroy();
		RightHandEquippedWeapon = nullptr;
		if (LeftHandEquippedWeapon)
		{
			InAttackMoveSpeed = LeftHandEquippedWeapon->GetWeaponAttackMoveSpeed();
		}
	}
	if (RightHandEquippedWeapon == nullptr && LeftHandEquippedWeapon == nullptr)
	{
		InAttackMoveSpeed = DefaultInAttackMoveSpeed;
	}
}

void UCombatComponent::UnequipLeftWeapon()
{
	if (LeftHandEquippedWeapon)
	{
		//const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(FName("RightHandSocket"));
		LeftHandEquippedWeapon->Destroy();
		LeftHandEquippedWeapon = nullptr;
		if (RightHandEquippedWeapon)
		{
			InAttackMoveSpeed = RightHandEquippedWeapon->GetWeaponAttackMoveSpeed();
		}
	}
	if (LeftHandEquippedWeapon == nullptr && RightHandEquippedWeapon == nullptr)
	{
		InAttackMoveSpeed = DefaultInAttackMoveSpeed;
	}
}