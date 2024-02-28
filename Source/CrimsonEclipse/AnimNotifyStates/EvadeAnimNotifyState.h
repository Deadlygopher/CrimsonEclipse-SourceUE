// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "EvadeAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class CRIMSONECLIPSE_API UEvadeAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	
private:
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation, float FrameDeltaTime) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
