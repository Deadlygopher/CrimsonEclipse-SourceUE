// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterLevelComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRIMSONECLIPSE_API UCharacterLevelComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCharacterLevelComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	int32 GetCurrentExpForKill() const { return CurrentExpForKill; }
	int32 GetCurrentLevel() const { return CurrentLevel; }

	void LevelUp();
	void CountCurrentExpForKill();

	int32 GetHealthPoints() const { return HealthPoints; }
	int32 GetStaminaPoints() const { return StaminaPoints; }
	int32 GetStrengthPoints() const { return StrengthPoints; }
	int32 GetDexterityPoints() const { return DexterityPoints; }
	int32 GetIntelligencePoints() const { return IntelligencePoints; }

	void AddHealthPoints(int32 NewHealthPoints) { HealthPoints += NewHealthPoints; }
	void AddStaminaPoints(int32 NewStaminaPoints) { StaminaPoints += NewStaminaPoints; }
	void AddStrengthPoints(int32 NewStrengthPoints) { StrengthPoints += NewStrengthPoints; }
	void AddDexterityPoints(int32 NewDexterityPoints) { DexterityPoints += NewDexterityPoints; }
	void AddIntelligencePoints(int32 NewIntelligencePoints) { IntelligencePoints += NewIntelligencePoints; }

	void AddFreeStatsPoints(int32 PointsToAdd) { FreeStatsPoints += PointsToAdd; }
	void CountFreeStatsPoints();
	int32 GetFreeStatsPoints() const { return FreeStatsPoints; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "EXP")
	int32 CurrentLevel = 1;

	UPROPERTY(EditDefaultsOnly, Category = "EXP")
	int32 InitExpForKill = 100;

	UPROPERTY()
	int32 CurrentExpForKill = 1;


	//////--------INIT-FREE-STAT-POINTS-------//////

	UPROPERTY(EditDefaultsOnly, Category = "InitStats")
	int32 InitFreeStatsPoints = 10;

	//////--------FREE-STAT-POINTS-------//////

	UPROPERTY(EditDefaultsOnly, Category = "InitStats")
	int32 FreeStatsPoints = 0;

	UPROPERTY(EditDefaultsOnly, Category = "InitStats")
	int32 FreeStatsPointsPerLevel = 1;

	//////--------INIT-STATS-------//////

	UPROPERTY(EditDefaultsOnly, Category = "InitStats")
	int32 InitHealthPoints = 10;

	UPROPERTY(EditDefaultsOnly, Category = "InitStats")
	int32 InitStaminaPoints = 10;

	UPROPERTY(EditDefaultsOnly, Category = "InitStats")
	int32 InitStrengthPoints = 10;

	UPROPERTY(EditDefaultsOnly, Category = "InitStats")
	int32 InitDexterityPoints = 10;

	UPROPERTY(EditDefaultsOnly, Category = "InitStats")
	int32 InitIntelligencePoints = 10;

	//////------CURRENT-STATS------///////

	UPROPERTY()
	int32 HealthPoints = InitHealthPoints;

	UPROPERTY()
	int32 StaminaPoints = InitStaminaPoints;

	UPROPERTY()
	int32 StrengthPoints= InitStrengthPoints;

	UPROPERTY()
	int32 DexterityPoints= InitDexterityPoints;

	UPROPERTY()
	int32 IntelligencePoints= InitIntelligencePoints;
};
