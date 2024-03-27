// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "HitDetectAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class CRIMSONECLIPSE_API UHitDetectAnimNotify : public UAnimNotifyState
{
	GENERATED_BODY()

private:

	UFUNCTION(BlueprintCallable)
	//void DetectHitOnTick(USkeletalMeshComponent* MeshComp);

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation, float FrameDeltaTime) override;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
};
