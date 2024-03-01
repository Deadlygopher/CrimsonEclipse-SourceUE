// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "CEAIPerceptionComponent.generated.h"

/**
 * 
 */
UCLASS()
class CRIMSONECLIPSE_API UCEAIPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()
public:
	AActor* GetClosestEnemy() const;

};
