// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CrimsonEclipse/Interfaces/XPComponentInterface.h"

#include "XPComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogXPComponent, Log, All);

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnReceiveExp, int32 CurrentLevelMaxExp, int32 CurrentLevelExp)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnLevelUp, int32 NewLevel)

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

	UFUNCTION(BlueprintCallable)
	void SetCurrentLevel(int32 LevelToSet);

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentLevel() const { return CurrentLevel; }

	FOnReceiveExp OnReceiveExp;
	FOnLevelUp OnLevelUp;

private:
	UPROPERTY(EditDefaultsOnly)
	int32 CurrentExp = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 InitLevelExp = 1000;

	UPROPERTY()
	int32 NextLevelExp = 1000;

	UPROPERTY()
	int32 CurrentLevelRequirment = InitLevelExp;

	UPROPERTY()
	int32 PrevLevelExp = 0;

	UPROPERTY()
	int32 CurrentLevel = 1;

	UPROPERTY(EditDefaultsOnly)
	float NextLevelExpModifier = 1.f;

	UPROPERTY(EditDefaultsOnly)
	float RequireGrowCoefficient = 0.05f;
};
