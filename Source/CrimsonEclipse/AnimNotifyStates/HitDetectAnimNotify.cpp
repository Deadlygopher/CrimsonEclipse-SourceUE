// Fill out your copyright notice in the Description page of Project Settings.


#include "HitDetectAnimNotify.h"
#include "CrimsonEclipse/Interfaces/HitDetectInterface.h"


void UHitDetectAnimNotify::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
	/*
	auto Interfaces = MeshComp->GetOwner()->GetComponents();
	for (auto Interface : Interfaces)
	{
		if (auto HitInterface = Cast<IHitDetectInterface>(Interface))
		{
			HitInterface->OnHitDetect();
			break;
		}
	}*/
	auto FoundInterface = MeshComp->GetOwner()->FindComponentByInterface<UHitDetectInterface>();
	auto FoundComponent = Cast<IHitDetectInterface>(FoundInterface);
	if (FoundComponent) FoundComponent->OnHitDetect();
}

void UHitDetectAnimNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	/*
	auto Interfaces = MeshComp->GetOwner()->GetComponents();
	for (auto Interface : Interfaces)
	{
		if (auto HitInterface = Cast<IHitDetectInterface>(Interface))
		{
			HitInterface->ResetTracingVectors();
			break;
		}
	}*/
	auto FoundInterface = MeshComp->GetOwner()->FindComponentByInterface<UHitDetectInterface>();
	auto FoundComponent = Cast<IHitDetectInterface>(FoundInterface);
	if (FoundComponent) FoundComponent->ResetTracingVectors();
}
