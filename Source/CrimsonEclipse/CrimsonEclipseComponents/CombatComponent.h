// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CrimsonEclipse/Interfaces/HitDetectInterface.h"

#include "CombatComponent.generated.h"

class AWeapon;
class ACEBaseCharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRIMSONECLIPSE_API UCombatComponent : public UActorComponent, public IHitDetectInterface
{
	GENERATED_BODY()

public:	
	UCombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	friend ACEBaseCharacter;

	void EquipRightWeapon(AWeapon* WeaponToEquip);
	void EquipLeftWeapon(AWeapon* WeaponToEquip);

	void UnequipRightWeapon();
	void UnequipLeftWeapon();

	virtual AWeapon* GetRightHandWeapon();
	virtual AWeapon* GetLeftHandWeapon();
	void SetCharacter(ACEBaseCharacter* Char);

	//Damage
	float GetRightHandDamage() { return RightHandDamage; }
	void SetRightHandDamage(float NewDamage) { RightHandDamage = NewDamage; }

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void ResetTracingVectors() override;

private:
	
	ACEBaseCharacter* Character;
	AWeapon* RightHandEquippedWeapon;
	AWeapon* LeftHandEquippedWeapon;

	TArray<AActor*> ActorsToIgnore;

	//Tracing Sockets for weapon
	FVector HalfSize{ 5.f, 5.f, 5.f };

	FVector PrevStartSocketLocation;
	FVector CurrentStartSocketLocation;

	FVector PrevMiddleSocketLocation;
	FVector CurrentMiddleSocketLocation;

	FVector PrevEndSocketLocation;
	FVector CurrentEndSocketLocation;

	// Damage
	float RightHandDamage = 1.f;

public:
	UFUNCTION()
	virtual void OnHitDetect() override;

	UFUNCTION()
	void SimpleRightHandAttack();
};
