// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "CrimsonEclipse/Items/Weapon.h"
#include "CrimsonEclipse/Character/CEBaseCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
//#include "CrimsonEclipse/Projectile/CEProjectileActor.h"

#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY(LogCombatComponent);

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	//SetIsReplicated(true);
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
			QueryArray, false, ActorsToIgnore, EDrawDebugTrace::ForDuration,
			HitResult, true, FLinearColor::Red, FLinearColor::Green, 2.f);

		UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), PrevMiddleSocketLocation, CurrentMiddleSocketLocation,
			QueryArray, false, ActorsToIgnore, EDrawDebugTrace::ForDuration,
			HitResult, true, FLinearColor::Red, FLinearColor::Green, 2.f);

		UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), PrevEndSocketLocation, CurrentEndSocketLocation,
			QueryArray, false, ActorsToIgnore, EDrawDebugTrace::ForDuration,
			HitResult, true, FLinearColor::Red, FLinearColor::Green, 2.f);

		UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), PrevEndSocketLocation, CurrentStartSocketLocation,
			QueryArray, false, ActorsToIgnore, EDrawDebugTrace::ForDuration,
			HitResult, true, FLinearColor::Red, FLinearColor::Green, 2.f);

		PrevStartSocketLocation = CurrentStartSocketLocation;
		PrevMiddleSocketLocation = CurrentMiddleSocketLocation;
		PrevEndSocketLocation = CurrentEndSocketLocation;

		if (HitResult.bBlockingHit)
		{
			UGameplayStatics::ApplyDamage(HitResult.GetActor(), RightHandDamage,
				Cast<APawn>(GetOwner())->GetController(), GetOwner(), UDamageType::StaticClass());
			ActorsToIgnore.Add(HitResult.GetActor());
		}
	}
}

void UCombatComponent::OnAiming()
{
	if (LeftHandEquippedWeapon)
	{
		if (Character->IsPlayerControlled())
		{
			Character->RotateToCursorDirecion();
			const FTransform SocketTransform = LeftHandEquippedWeapon->GetWeaponMesh()->GetSocketTransform("ArrowSocket");
			const FVector TraceStart = FVector(Character->GetActorLocation().X, Character->GetActorLocation().Y, Character->GetActorLocation().Z+40);// Magic number

			const FVector ArrowDirection = Character->GetActorForwardVector();
			const FVector TraceEnd = TraceStart + ArrowDirection * 1000; // Magic number

			DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 3.f, 0, 3.f);
			//LeftHandEquippedWeapon->SpawnProjectile(Character, ArrowDirection);
		}
	}
}

void UCombatComponent::OnProjectileSpawn()
{
	if (LeftHandEquippedWeapon)
	{
		const FVector ArrowDirection = Character->GetActorForwardVector();
		LeftHandEquippedWeapon->SpawnProjectile(Character, ArrowDirection);
	}
}

void UCombatComponent::SimpleRightHandAttack()
{
	//ResetTracingVectors();
	//HitRightHandTracing();
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