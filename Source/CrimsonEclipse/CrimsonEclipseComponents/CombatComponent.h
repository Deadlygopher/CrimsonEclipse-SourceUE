// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CrimsonEclipse/Interfaces/HitDetectInterface.h"

#include "CombatComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCombatComponent, Log, All);

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

	float GetInAttackMoveSpeed() { return InAttackMoveSpeed;}

	//Weapon getters
	virtual AWeapon* GetRightHandWeapon();
	virtual AWeapon* GetLeftHandWeapon();

	void SetCharacter(ACEBaseCharacter* Char);

	//Damage
	float GetRightHandDamage() const { return RightHandDamage; }
	void SetRightHandDamage(float NewDamage) { RightHandDamage = NewDamage; }

	float GetLeftHandDamage() const { return LeftHandDamage; }
	void SetLeftHandDamage(float NewDamage) { LeftHandDamage = NewDamage; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void ResetTracingVectors() override;

	UFUNCTION()
	virtual void WeaponMakeSound() override;

private:
	
	UPROPERTY()
	ACEBaseCharacter* Character;

	UPROPERTY()
	AWeapon* RightHandEquippedWeapon;

	UPROPERTY()
	AWeapon* LeftHandEquippedWeapon;

	UPROPERTY()
	TArray<AActor*> ActorsToIgnore;

	UPROPERTY(Replicated)
	FVector ProjectileVector;

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
	float LeftHandDamage = 1.f;
	float DefaultInAttackMoveSpeed = 0.f;
	float InAttackMoveSpeed = 0.f;

public:
	UFUNCTION()
	virtual void OnHitDetect() override;

	// ON AIMING REPLICATION //
	UFUNCTION()
	virtual void OnAiming() override;

	UFUNCTION(Client, Reliable)
	void Client_OnAiming();

	UFUNCTION(Server, Reliable)
	void Server_OnAiming();
	// ON AIMING REPLICATION //


	// ON PROJECTILE SPAWN REPLICATION //
	UFUNCTION()
	virtual void OnProjectileSpawn() override;

	UFUNCTION()
	void MC_OnProjectileSpawn(FVector VectorToSpawn);

	UFUNCTION(Server, Reliable)
	void Server_OnProjectileSpawn(FVector VectorToSpawn);

	UFUNCTION(Client, Reliable)
	void Client_ProjectileVector();
	// ON PROJECTILE SPAWN REPLICATION //


	UFUNCTION()
	void LightAttack();

	UFUNCTION()
	void HeavyAttack();


	/// PLAY ANIMATION ///
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayAttackMontage(UAnimMontage* AnimMontage);

	UFUNCTION(Server, Reliable)
	void Server_PlayAttackMontage(UAnimMontage* AnimMontage);
	/// PLAY ANIMATION ///


	UFUNCTION(Server, Reliable)
	void Server_ApplyDamageToActor(const FHitResult& HitResult);

	UFUNCTION(Client, Reliable)
	void Client_ApplyDamageToActor(const FHitResult& HitResult);
};
