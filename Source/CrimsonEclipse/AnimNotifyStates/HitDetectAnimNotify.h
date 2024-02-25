// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CrimsonEclipse/Interfaces/HitDetectInterface.h"
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
	void DetectHitOnTick(USkeletalMeshComponent* MeshComp);

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
};
