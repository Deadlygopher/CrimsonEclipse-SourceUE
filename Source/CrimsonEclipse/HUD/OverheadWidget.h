// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverheadWidget.generated.h"

/**
 * 
 */
UCLASS()
class CRIMSONECLIPSE_API UOverheadWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void UpdateHealthBar(float Health, float MaxHealth) const;


protected:
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DisplayHealth;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;
};
