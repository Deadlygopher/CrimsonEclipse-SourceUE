// Fill out your copyright notice in the Description page of Project Settings.


#include "HitDetectAnimNotify.h"

void UHitDetectAnimNotify::DetectHitOnTick(USkeletalMeshComponent* MeshComp)
{
	auto Interfaces = MeshComp->GetOwner()->GetComponents();
	for (auto Interface : Interfaces)
	{
		if (auto HitInterface = Cast<IHitDetectInterface>(Interface))
		{
			HitInterface->OnHitDetect();
			break;
		}
	}
}

void UHitDetectAnimNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	auto Interfaces = MeshComp->GetOwner()->GetComponents();
	for (auto Interface : Interfaces)
	{
		if (auto HitInterface = Cast<IHitDetectInterface>(Interface))
		{
			HitInterface->ResetTracingVectors();
			break;
		}
	}
}
