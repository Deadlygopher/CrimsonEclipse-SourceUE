// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatsWidget.h"


void APlayerHUD::BeginPlay()
{
	PlayerStats = Cast<UPlayerStatsWidget>(CreateWidget(GetOwningPlayerController(), PlayerStatsWidget, "PlayerStats"));
	PlayerStats->AddToViewport();
	PlayerStats->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void APlayerHUD::SetPlayerStats(float Health, float MaxHealth)
{
	PlayerStats->UpdatePlayerWidget(Health, MaxHealth);
}
