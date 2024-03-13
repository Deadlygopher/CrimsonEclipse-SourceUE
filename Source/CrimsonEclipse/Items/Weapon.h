// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class USphereComponent;
class ACEProjectileActor;

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial		UMETA(DisplayName = "Initial State"),
	EWS_Equipped	UMETA(DisplayName = "Equipped"),
	EWS_Dropped		UMETA(DisplayName = "Dropped"),

	EWS_MAX UMETA(DisplayName= "DefaultMax")
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_Melee		UMETA(DisplayName = "Melee"),
	EWT_Range		UMETA(DisplayName = "Range"),
	EWT_Magic		UMETA(DisplayName = "Magic"),

	EWT_MAX			UMETA(DisplayName = "DefaultMax")
};

UCLASS()
class CRIMSONECLIPSE_API AWeapon : public AActor
{
	GENERATED_BODY()

public:	
	AWeapon();
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	float GetWeaponAttackMoveSpeed() { return WeaponAttackMoveSpeed; }

	UAnimMontage* GetLightAttackAnimMontage() { return WeaponLightAttackAnimMontage; }
	USkeletalMeshComponent* GetWeaponMesh() { return WeaponMesh; }
	EWeaponType GetWeaponType() { return WeaponType; }

	void SpawnProjectile(APawn* SpawnInstigator, FVector AimVector);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(/*ReplicatedUsing = OnRep_WeaponState,*/ VisibleAnywhere, Category = "Weapon Properties")
	EWeaponState WeaponState;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
	EWeaponType WeaponType;

	//UFUNCTION()
	//void OnRep_WeaponState();

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
	UAnimMontage* WeaponLightAttackAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
	UAnimMontage* WeaponHeavyAttackAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
	float WeaponAttackMoveSpeed = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
	TSubclassOf<ACEProjectileActor> ProjectileClass;
};
