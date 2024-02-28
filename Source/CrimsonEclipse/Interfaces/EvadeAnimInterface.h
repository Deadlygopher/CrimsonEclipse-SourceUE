// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EvadeAnimInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEvadeAnimInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CRIMSONECLIPSE_API IEvadeAnimInterface
{
	GENERATED_BODY()

public:
	virtual void StopRoll();
	virtual void RollInProcess();
};
