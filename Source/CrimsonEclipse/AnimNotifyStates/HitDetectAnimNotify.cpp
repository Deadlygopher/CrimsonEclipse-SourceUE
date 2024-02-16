// Fill out your copyright notice in the Description page of Project Settings.


#include "HitDetectAnimNotify.h"

void UHitDetectAnimNotify::DetectHitOnTick(USkeletalMeshComponent* MeshComp)
{
	IHitDetectInterface* HitInterface = Cast<IHitDetectInterface>(MeshComp->GetOwner());
	if (HitInterface)
	{
		HitInterface->OnHitDetect();
	}
}
