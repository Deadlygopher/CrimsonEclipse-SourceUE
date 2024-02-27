// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChange, float Health, float MaxHealth)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRIMSONECLIPSE_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();

	float GetHealth() const { return Health; }
	float GetMaxHealth() const { return MaxHealth; }

	UFUNCTION(BlueprintCallable)
	void IncreaseHealth(float HealthToIncrease);

	UFUNCTION(BlueprintCallable)
	void DecreaseHealth(float HealthToDecrease);

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxHealth;

	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	float Health = 50.f;

public:
	FOnHealthChange OnHealthChange;
};
