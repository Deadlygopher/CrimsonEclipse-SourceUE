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

void UCombatComponent::HitTracing()
{
	if(RightHandEquippedWeapon)
	{
		FVector SocketLocation = RightHandEquippedWeapon->GetWeaponMesh()->GetSocketLocation("StartTrackingSocket");
		UWorld* World = GetWorld();

		TArray<TEnumAsByte<EObjectTypeQuery>> QueryArray;
		QueryArray.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(GetOwner());

		FHitResult HitResult;

		if (World)
		{
			UKismetSystemLibrary::SphereTraceSingleForObjects(World, SocketLocation, SocketLocation,
				50.f, QueryArray, false, ActorsToIgnore, EDrawDebugTrace::ForDuration,
				HitResult, true, FLinearColor::Red, FLinearColor::Green, 2.f);
		}
	}
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