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

void UCombatComponent::EquipWeapon(AWeapon* WeaponToEquip)
{
	if (Character == nullptr || WeaponToEquip == nullptr) return;

	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);

	const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(FName("RightHandSocket"));
	if (HandSocket)
	{
		HandSocket->AttachActor(EquippedWeapon, Character->GetMesh());
	}
	EquippedWeapon->SetOwner(Character);
	//EquippedWeapon->ShowPickupWidget(false);
	//EquippedWeapon->GetAreaSphere()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UCombatComponent::HitTracing()
{
	if(EquippedWeapon)
	{
		FVector SocketLocation = EquippedWeapon->GetWeaponMesh()->GetSocketLocation("RightHandSocket");
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
