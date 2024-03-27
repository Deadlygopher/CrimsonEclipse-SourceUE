// Fill out your copyright notice in the Description page of Project Settings.


#include "AimingAnimNotifyState.h"
#include "CrimsonEclipse/Interfaces/HitDetectInterface.h"

void UAimingAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
	/*
	auto Interfaces = MeshComp->GetOwner()->GetComponents();
	for (auto Interface : Interfaces)
	{
		if (auto HitInterface = Cast<IHitDetectInterface>(Interface))
		{
			HitInterface->OnAiming();
			break;
		}
	}*/
	auto FoundInterface = MeshComp->GetOwner()->FindComponentByInterface<UHitDetectInterface>();
	auto FoundComponent = Cast<IHitDetectInterface>(FoundInterface);
	if (FoundComponent) FoundComponent->OnAiming();
}

void UAimingAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	/*
	auto Interfaces = MeshComp->GetOwner()->GetComponents();
	for (auto Interface : Interfaces)
	{
		if (auto HitInterface = Cast<IHitDetectInterface>(Interface))
		{
			HitInterface->OnProjectileSpawn();
			break;
		}
	}*/
	auto FoundInterface = MeshComp->GetOwner()->FindComponentByInterface<UHitDetectInterface>();
	auto FoundComponent = Cast<IHitDetectInterface>(FoundInterface);
	if (FoundComponent) FoundComponent->OnProjectileSpawn();
}
