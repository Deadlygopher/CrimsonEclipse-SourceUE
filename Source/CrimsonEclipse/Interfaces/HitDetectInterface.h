// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HitDetectInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHitDetectInterface : public UInterface
{
	GENERATED_BODY()
};

class AWeapon;

class CRIMSONECLIPSE_API IHitDetectInterface
{
	GENERATED_BODY()

public:

	// Melee Weapon
	virtual void OnHitDetect();
	virtual void ResetTracingVectors();
	// Melee Weapon

	// Range Weapon
	virtual void OnAiming();
	virtual void OnProjectileSpawn();
	// Range Weapon

	virtual void WeaponMakeSound();
	virtual void WeaponCompleteSpawnTrail();
};
