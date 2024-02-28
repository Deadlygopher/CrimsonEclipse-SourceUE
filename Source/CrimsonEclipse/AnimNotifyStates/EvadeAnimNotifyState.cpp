// Fill out your copyright notice in the Description page of Project Settings.


#include "EvadeAnimNotifyState.h"
#include "CrimsonEclipse/Interfaces/EvadeAnimInterface.h"



void UEvadeAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	if (Cast<IEvadeAnimInterface>(MeshComp->GetOwner()))
	{
		Cast<IEvadeAnimInterface>(MeshComp->GetOwner())->RollInProcess();
	}
}

void UEvadeAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	if(Cast<IEvadeAnimInterface>(MeshComp->GetOwner()))
	{
		Cast<IEvadeAnimInterface>(MeshComp->GetOwner())->StopRoll();
	}
}
