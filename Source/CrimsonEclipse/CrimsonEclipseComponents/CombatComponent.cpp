// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "CrimsonEclipse/Items/Weapon.h"
#include "CrimsonEclipse/Character/CrimsonEclipseCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicated(true);
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
}

void UCombatComponent::EquipRightWeapon(AWeapon* WeaponToEquip)
{
	if (Character == nullptr || WeaponToEquip == nullptr) return;

	RightHandEquippedWeapon = WeaponToEquip;

	if (RightHandEquippedWeapon) {
		RightHandEquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);

		const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(FName("RightHandSocket"));
		if (HandSocket)
		{
			HandSocket->AttachActor(RightHandEquippedWeapon, Character->GetMesh());
		}
		RightHandEquippedWeapon->SetOwner(Character);
		RightHandEquippedWeapon->SetActorLocationAndRotation(RightHandEquippedWeapon->GetWeaponMesh()->GetSocketLocation("HandleSocket"),
			RightHandEquippedWeapon->GetWeaponMesh()->GetSocketRotation("HandleSocket"));
	}
}

void UCombatComponent::EquipLeftWeapon(AWeapon* WeaponToEquip)
{
	if (Character == nullptr || WeaponToEquip == nullptr) return;

	LeftHandEquippedWeapon = WeaponToEquip;

	if (LeftHandEquippedWeapon)
	{
		LeftHandEquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);

		const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(FName("LeftHandSocket"));
		if (HandSocket)
		{
			HandSocket->AttachActor(LeftHandEquippedWeapon, Character->GetMesh());
		}
		LeftHandEquippedWeapon->SetOwner(Character);
		LeftHandEquippedWeapon->SetActorLocationAndRotation(LeftHandEquippedWeapon->GetWeaponMesh()->GetSocketLocation("HandleSocket"),
			LeftHandEquippedWeapon->GetWeaponMesh()->GetSocketRotation("HandleSocket"));
	}
}

void UCombatComponent::ResetTracingVectors()
{
	if (!RightHandEquippedWeapon) { return; }
	PrevStartSocketLocation = CurrentStartSocketLocation = RightHandEquippedWeapon->GetWeaponMesh()->GetSocketLocation("StartTrackingSocket");
	PrevMiddleSocketLocation = CurrentMiddleSocketLocation = RightHandEquippedWeapon->GetWeaponMesh()->GetSocketLocation("MiddleTrackingSocket");
	PrevEndSocketLocation = CurrentEndSocketLocation = RightHandEquippedWeapon->GetWeaponMesh()->GetSocketLocation("EndTrackingSocket");
}

void UCombatComponent::OnHitDetect()
{
	if(RightHandEquippedWeapon)
	{
		FVector StartSocketLocation = RightHandEquippedWeapon->GetWeaponMesh()->GetSocketLocation("StartTrackingSocket");
		FVector MiddleSocketLocation = RightHandEquippedWeapon->GetWeaponMesh()->GetSocketLocation("MiddleTrackingSocket");
		FVector EndSocketLocation = RightHandEquippedWeapon->GetWeaponMesh()->GetSocketLocation("EndTrackingSocket");
		//FRotator StartSocketOrientation = StartSocketLocation.ToOrientationRotator();
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
			CurrentMiddleSocketLocation = MiddleSocketLocation;
			CurrentEndSocketLocation = EndSocketLocation;

			UKismetSystemLibrary::LineTraceMultiForObjects(World, PrevStartSocketLocation, CurrentStartSocketLocation,
				QueryArray, false, ActorsToIgnore, EDrawDebugTrace::ForDuration,
				MultiHitResult, true, FLinearColor::Red, FLinearColor::Green, 2.f);
			UKismetSystemLibrary::LineTraceMultiForObjects(World, PrevMiddleSocketLocation, CurrentMiddleSocketLocation,
				QueryArray, false, ActorsToIgnore, EDrawDebugTrace::ForDuration,
				MultiHitResult, true, FLinearColor::Red, FLinearColor::Green, 2.f);
			UKismetSystemLibrary::LineTraceMultiForObjects(World, PrevEndSocketLocation, CurrentEndSocketLocation,
				QueryArray, false, ActorsToIgnore, EDrawDebugTrace::ForDuration,
				MultiHitResult, true, FLinearColor::Red, FLinearColor::Green, 2.f);

			UKismetSystemLibrary::LineTraceMultiForObjects(World, PrevEndSocketLocation, CurrentStartSocketLocation,
				QueryArray, false, ActorsToIgnore, EDrawDebugTrace::ForDuration,
				MultiHitResult, true, FLinearColor::Red, FLinearColor::Green, 2.f);

			PrevStartSocketLocation = CurrentStartSocketLocation;
			PrevMiddleSocketLocation = CurrentMiddleSocketLocation;
			PrevEndSocketLocation = CurrentEndSocketLocation;
		}
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