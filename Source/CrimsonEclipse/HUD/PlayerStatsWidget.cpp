// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatsWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayerStatsWidget::UpdatePlayerWidget(float Health, float MaxHealth)
{
	if(MaxHealth>0)
	{
		PlayerHealthBar->SetPercent(Health / MaxHealth);
		PlayerHealthText->SetText(FText::FromString(FString::FromInt(Health)));
	}
}
