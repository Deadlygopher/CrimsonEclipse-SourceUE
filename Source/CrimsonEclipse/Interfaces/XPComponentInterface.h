// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "XPComponentInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UXPComponentInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CRIMSONECLIPSE_API IXPComponentInterface
{
	GENERATED_BODY()

public:
	virtual void ReceiveExp(int32 Exp);
};
