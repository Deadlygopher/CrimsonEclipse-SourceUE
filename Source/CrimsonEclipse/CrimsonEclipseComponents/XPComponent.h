// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CrimsonEclipse/Interfaces/XPComponentInterface.h"

#include "XPComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogXPComponent, Log, All);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRIMSONECLIPSE_API UXPComponent : public UActorComponent, public IXPComponentInterface
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
	void ReceiveExp(int32 ExpToAdd) override;

	UFUNCTION(BlueprintCallable)
	void SetExpForNextLevel(int32 NewLevel);

private:
	UPROPERTY(EditDefaultsOnly)
	int32 CurrentExp = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 NextLevelExp = 1000;

	UPROPERTY()
	int32 CurrentLevelRequirment = 0;

	UPROPERTY()
	int32 PrevLevelExp = 0;

	UPROPERTY(EditDefaultsOnly)
	float NextLevelExpModifier = 1.f;

	UPROPERTY(EditDefaultsOnly)
	float RequireGrowCoefficient = 0.05;
};
