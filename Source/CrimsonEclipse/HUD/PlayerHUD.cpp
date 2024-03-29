// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatsWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "PlayerStatPoints.h"


void APlayerHUD::BeginPlay()
{
	PlayerStats = Cast<UPlayerStatsWidget>(CreateWidget(GetOwningPlayerController(), PlayerStatsWidgetClass, "PlayerStats"));
	PlayerStats->AddToViewport();
	PlayerStats->SetVisibility(ESlateVisibility::HitTestInvisible);
	GetOwningPlayerController()->InputComponent->BindAction("OpenCharacterStats", IE_Pressed, this, &APlayerHUD::CreatePlayerStatPointsWidget);
}

void APlayerHUD::UpdateHUDHealth(float Health, float MaxHealth)
{
	if (MaxHealth > 0 && PlayerStats && PlayerStats->PlayerHealthBar && PlayerStats->PlayerHealthText)
	{
		PlayerStats->PlayerHealthBar->SetPercent(Health / MaxHealth);
		PlayerStats->PlayerHealthText->SetText(FText::FromString(FString::FromInt(Health)));
	}
}

void APlayerHUD::UpdateHUDExpBar(int32 CurrentLevelMaxExp, int32 CurrentLevelExp)
{
	if (PlayerStats && PlayerStats->ExpProgressBar)
	{
		float MaxExp = CurrentLevelMaxExp;
		float CurentExp = CurrentLevelExp;
		PlayerStats->ExpProgressBar->SetPercent(CurentExp / MaxExp);
	}
	if (PlayerStats && PlayerStats->MaxExpForLevel && PlayerStats->CurrentExpForLevel)
	{
		PlayerStats->MaxExpForLevel->SetText(FText::FromString(FString::FromInt(CurrentLevelMaxExp)));
		PlayerStats->CurrentExpForLevel->SetText(FText::FromString(FString::FromInt(CurrentLevelExp)));
	}
}

void APlayerHUD::UpdateHUDLevelText(int32 NewLevel)
{
	if (PlayerStats && PlayerStats->CurrentLevelText)
	{
		PlayerStats->CurrentLevelText->SetText(FText::FromString(FString::FromInt(NewLevel)));
	}
}

void APlayerHUD::CreatePlayerStatPointsWidget()
{
	if (!PlayerStatPoints && PlayerStatPointsClass)
	{
		PlayerStatPoints = Cast<UPlayerStatPoints>(CreateWidget(GetOwningPlayerController(), PlayerStatPointsClass, "PlayerStatPoints"));
	}
	if (PlayerStatPoints)
	{
		if (!PlayerStatPoints->IsInViewport())
		{
			PlayerStatPoints->AddToViewport();
		}
		else
		{
			if (!PlayerStatPoints->IsVisible())
			{
				PlayerStatPoints->UpdateWidget();
				PlayerStatPoints->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				PlayerStatPoints->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}