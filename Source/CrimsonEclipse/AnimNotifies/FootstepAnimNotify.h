// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "FootstepAnimNotify.generated.h"

class USoundCue;

UCLASS()
class CRIMSONECLIPSE_API UFootstepAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	void Notify (USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
	UPROPERTY(EditDefaultsOnly)
	USoundCue* DefaultSound;
};
