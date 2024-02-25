// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "CrimsonEclipse/Items/Weapon.h"
#include "CrimsonEclipse/CrimsonEclipseCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
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

void UCombatComponent::EquipRightWeapon(AWeapon* WeaponToEquip)
{
	if (Character == nullptr || WeaponToEquip == nullptr) return;

	RightHandEquippedWeapon = WeaponToEquip;
	RightHandEquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);

	const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(FName("RightHandSocket"));
	if (HandSocket)
	{
		HandSocket->AttachActor(RightHandEquippedWeapon, Character->GetMesh());
	}
	RightHandEquippedWeapon->SetOwner(Character);
}

void UCombatComponent::EquipLeftWeapon(AWeapon* WeaponToEquip)
{
	if (Character == nullptr || WeaponToEquip == nullptr) return;

	LeftHandEquippedWeapon = WeaponToEquip;
	LeftHandEquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);

	const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(FName("LeftHandSocket"));
	if (HandSocket)
	{
		HandSocket->AttachActor(LeftHandEquippedWeapon, Character->GetMesh());
	}
	LeftHandEquippedWeapon->SetOwner(Character);
}

void UCombatComponent::ResetTracingVectors()
{
	if (!RightHandEquippedWeapon) { return; }
	PrevStartSocketLocation = CurrentStartSocketLocation = RightHandEquippedWeapon->GetWeaponMesh()->GetSocketLocation("StartTrackingSocket");
}

void UCombatComponent::OnHitDetect()
{
	if(RightHandEquippedWeapon)
	{
		FVector StartSocketLocation = RightHandEquippedWeapon->GetWeaponMesh()->GetSocketLocation("StartTrackingSocket");
		FVector EndSocketLocation = RightHandEquippedWeapon->GetWeaponMesh()->GetSocketLocation("EndTrackingSocket");
		FRotator StartSocketOrientation = StartSocketLocation.ToOrientationRotator();
		UWorld* World = GetWorld();

		TArray<TEnumAsByte<EObjectTypeQuery>> QueryArray;
		QueryArray.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(GetOwner());

		FHitResult HitResult;
		TArray<FHitResult> MultiHitResult;

		if (World)
		{
			CurrentStartSocketLocation = StartSocketLocation;

			UKismetSystemLibrary::BoxTraceMultiForObjects(World, StartSocketLocation, EndSocketLocation,
				HalfSize, StartSocketOrientation, QueryArray, false, ActorsToIgnore, EDrawDebugTrace::ForDuration,
				MultiHitResult, true, FLinearColor::Red, FLinearColor::Green, 2.f);

			if(CurrentStartSocketLocation == PrevStartSocketLocation)
			{
				PrevStartSocketLocation = CurrentStartSocketLocation = RightHandEquippedWeapon->GetWeaponMesh()->GetSocketLocation("StartTrackingSocket");
			}
			else 
			{
				UKismetSystemLibrary::LineTraceMultiForObjects(World, PrevStartSocketLocation, CurrentStartSocketLocation,
					QueryArray, false, ActorsToIgnore, EDrawDebugTrace::ForDuration,
					MultiHitResult, true, FLinearColor::Red, FLinearColor::Green, 2.f);
				PrevStartSocketLocation = CurrentStartSocketLocation;
			}
		}
		/* //Shere Trace
		if (World)
		{
			UKismetSystemLibrary::SphereTraceSingleForObjects(World, SocketLocation, SocketLocation,
				50.f, QueryArray, false, ActorsToIgnore, EDrawDebugTrace::ForDuration,
				HitResult, true, FLinearColor::Red, FLinearColor::Green, 2.f);
		}
		*/
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
		const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(FName("RightHandSocket"));
		RightHandEquippedWeapon->Destroy();
		RightHandEquippedWeapon = nullptr;
	}
}

void UCombatComponent::UnequipLeftWeapon()
{
	if (LeftHandEquippedWeapon)
	{
		const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(FName("RightHandSocket"));
		LeftHandEquippedWeapon->Destroy();
		LeftHandEquippedWeapon = nullptr;
	}
}