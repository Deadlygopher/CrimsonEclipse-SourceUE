// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatsWidget.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class CRIMSONECLIPSE_API UPlayerStatsWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdatePlayerWidget(float Health, float MaxHealth);

protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* PlayerHealthBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerHealthText;
};
