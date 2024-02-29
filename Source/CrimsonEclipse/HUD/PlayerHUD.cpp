// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatsWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


void APlayerHUD::BeginPlay()
{
	PlayerStats = Cast<UPlayerStatsWidget>(CreateWidget(GetOwningPlayerController(), PlayerStatsWidget, "PlayerStats"));
	PlayerStats->AddToViewport();
	PlayerStats->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void APlayerHUD::UpdateHUDHealth(float Health, float MaxHealth)
{
	if (MaxHealth > 0 && PlayerStats && PlayerStats->PlayerHealthBar && PlayerStats->PlayerHealthText)
	{
		PlayerStats->PlayerHealthBar->SetPercent(Health / MaxHealth);
		PlayerStats->PlayerHealthText->SetText(FText::FromString(FString::FromInt(Health)));
	}
}
