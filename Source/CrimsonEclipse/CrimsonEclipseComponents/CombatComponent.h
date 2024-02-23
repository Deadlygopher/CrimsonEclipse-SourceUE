// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

class AWeapon;
class ACrimsonEclipseCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRIMSONECLIPSE_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	friend class ACrimsonEclipseCharacter;

	void EquipRightWeapon(AWeapon* WeaponToEquip);
	void EquipLeftWeapon(AWeapon* WeaponToEquip);

	void UnequipRightWeapon();
	void UnequipLeftWeapon();

	virtual AWeapon* GetRightHandWeapon();
	virtual AWeapon* GetLeftHandWeapon();

protected:
	virtual void BeginPlay() override;

private:
	
	ACrimsonEclipseCharacter* Character;
	AWeapon* RightHandEquippedWeapon;
	AWeapon* LeftHandEquippedWeapon;

public:

	UFUNCTION()
	virtual void HitTracing();
};
