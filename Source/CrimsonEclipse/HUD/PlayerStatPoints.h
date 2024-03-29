// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatPoints.generated.h"

class UTextBlock;
class UButton;


UCLASS()
class CRIMSONECLIPSE_API UPlayerStatPoints : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void UpdateWidget();

protected:
	void NativeConstruct()override;

	UFUNCTION()
	void IncreaseHealthPoints();
	UFUNCTION()
	void IncreaseStaminaPoints();
	UFUNCTION()
	void IncreaseStrengthPoints();
	UFUNCTION()
	void IncreaseDexterityPoints();
	UFUNCTION()
	void IncreaseIntelligencePoints();

	UFUNCTION()
	void SetFreePointsCount(int32 CurrentLevel);

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* FreePointsCount;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HealthPoints;
	UPROPERTY(meta = (BindWidget))
	UButton* HealthPointsPlus;
	UPROPERTY(meta = (BindWidget))
	UButton* HealthPointsMinus;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* StaminaPoints;
	UPROPERTY(meta = (BindWidget))
	UButton* StaminaPointsPlus;
	UPROPERTY(meta = (BindWidget))
	UButton* StaminaPointsMinus;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* StrengthPoints;
	UPROPERTY(meta = (BindWidget))
	UButton* StrengthPointsPlus;
	UPROPERTY(meta = (BindWidget))
	UButton* StrengthPointsMinus;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DexterityPoints;
	UPROPERTY(meta = (BindWidget))
	UButton* DexterityPointsPlus;
	UPROPERTY(meta = (BindWidget))
	UButton* DexterityPointsMinus;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* IntelligencePoints;
	UPROPERTY(meta = (BindWidget))
	UButton* IntelligencePointsPlus;
	UPROPERTY(meta = (BindWidget))
	UButton* IntelligencePointsMinus;
};
