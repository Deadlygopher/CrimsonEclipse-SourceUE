// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AimingAnimNotifyState.generated.h"


UCLASS()
class CRIMSONECLIPSE_API UAimingAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation, float FrameDeltaTime) override;
};
