// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "PlayerHUD.generated.h"

class UPlayerStatsWidget;

UCLASS()
class CRIMSONECLIPSE_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual void UpdateHUDHealth(float Health, float MaxHealth);
	virtual void UpdateHUDExpBar(int32 CurrentLevelMaxExp, int32 CurrentLevelExp);
	virtual void UpdateHUDLevelText(int32 NewLevel);

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf <UPlayerStatsWidget> PlayerStatsWidgetClass;

	UPlayerStatsWidget* PlayerStats;
};
