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
	UPROPERTY(meta = (BindWidget))
	UProgressBar* PlayerHealthBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerHealthText;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* ExpProgressBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentLevelText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentExpForLevel;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MaxExpForLevel;
};
