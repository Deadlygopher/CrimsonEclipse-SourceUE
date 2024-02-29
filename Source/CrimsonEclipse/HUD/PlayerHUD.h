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

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf <UPlayerStatsWidget> PlayerStatsWidget;

	UPlayerStatsWidget* PlayerStats;
};
