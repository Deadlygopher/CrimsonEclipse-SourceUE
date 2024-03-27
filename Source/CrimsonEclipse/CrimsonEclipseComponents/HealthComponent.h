// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChange, float Health, float MaxHealth)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDeath, AActor* DamageCauser)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRIMSONECLIPSE_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();
	virtual void BeginPlay() override;

	float GetHealth() const { return Health; }
	float GetMaxHealth() const { return MaxHealth; }
	bool IsDead() const { return Health <= 0; }

	UFUNCTION(BlueprintCallable)
	void IncreaseHealth(float HealthToIncrease, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable)
	void DecreaseHealth(float HealthToDecrease, AActor* DamageCauser);

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxHealth;

private:
	UPROPERTY(EditAnywhere)
	float Health = 50.f;

public:
	FOnHealthChange OnHealthChange;
	FOnDeath OnDeath;
};
