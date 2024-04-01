// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CEProjectileActor.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogProjectileActor, Log, All);

class UProjectileMovementComponent;
class UCapsuleComponent;

UCLASS()
class CRIMSONECLIPSE_API ACEProjectileActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACEProjectileActor();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SetWeaponDamage(float BaseDamage) { ProjectileDamage = BaseDamage * DamageModifier; }

	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	UProjectileMovementComponent* ProjectileComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditDefaultsOnly)
	UCapsuleComponent* CapsuleComponent;

private:
	UPROPERTY(EditDefaultsOnly)
	float DamageModifier = 1.1;

	UPROPERTY()
	float ProjectileDamage = 1;
};
