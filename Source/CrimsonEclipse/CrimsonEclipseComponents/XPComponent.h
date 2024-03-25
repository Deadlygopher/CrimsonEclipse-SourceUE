// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "XPComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRIMSONECLIPSE_API UXPComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UXPComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void IncreaseCurrentXP(int32 ExpToAdd);

	UFUNCTION(BlueprintCallable)
	void SetExpForNextLevel(int32 NewLevel);

private:
	UPROPERTY(EditDefaultsOnly)
	int32 CurrentExp = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 NextLevelExp = 1000;

	UPROPERTY(EditDefaultsOnly)
	float NextLevelExpModifier = 1.f;

	UPROPERTY(EditDefaultsOnly)
	float RequirGrowCoefficient = 0.05;
};
