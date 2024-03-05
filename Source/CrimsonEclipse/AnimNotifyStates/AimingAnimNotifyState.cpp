// Fill out your copyright notice in the Description page of Project Settings.


#include "AimingAnimNotifyState.h"
#include "CrimsonEclipse/Interfaces/HitDetectInterface.h"

void UAimingAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	auto Interfaces = MeshComp->GetOwner()->GetComponents();
	for (auto Interface : Interfaces)
	{
		if (auto HitInterface = Cast<IHitDetectInterface>(Interface))
		{
			HitInterface->OnAiming();
			break;
		}
	}
}
