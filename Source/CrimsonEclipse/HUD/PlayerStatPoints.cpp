// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatPoints.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "CrimsonEclipse/CrimsonEclipseComponents/CharacterLevelComponent.h"
#include "CrimsonEclipse/CrimsonEclipseComponents/XPComponent.h"

void UPlayerStatPoints::NativeConstruct()
{
	HealthPointsPlus->OnClicked.AddDynamic(this, &UPlayerStatPoints::IncreaseHealthPoints);
	StaminaPointsPlus->OnClicked.AddDynamic(this, &UPlayerStatPoints::IncreaseStaminaPoints);
	StrengthPointsPlus->OnClicked.AddDynamic(this, &UPlayerStatPoints::IncreaseStrengthPoints);
	DexterityPointsPlus->OnClicked.AddDynamic(this, &UPlayerStatPoints::IncreaseDexterityPoints);
	IntelligencePointsPlus->OnClicked.AddDynamic(this, &UPlayerStatPoints::IncreaseIntelligencePoints);

	auto CharacterXPComp = GetOwningPlayerPawn()->FindComponentByClass<UXPComponent>();
	if (CharacterXPComp) CharacterXPComp->OnLevelUp.AddUObject(this, &UPlayerStatPoints::SetFreePointsCount);

	UpdateWidget();
}

void UPlayerStatPoints::IncreaseHealthPoints()
{
	auto CharacterLvlComp = GetOwningPlayerPawn()->FindComponentByClass<UCharacterLevelComponent>();
	if (CharacterLvlComp && CharacterLvlComp->GetFreeStatsPoints()>0)
	{
		CharacterLvlComp->AddHealthPoints(1);
		CharacterLvlComp->AddFreeStatsPoints(-1);
		HealthPoints->SetText(FText::FromString(FString::FromInt(CharacterLvlComp->GetHealthPoints())));
		FreePointsCount->SetText(FText::FromString(FString::FromInt(CharacterLvlComp->GetFreeStatsPoints())));
	}
}

void UPlayerStatPoints::IncreaseStaminaPoints()
{
	auto CharacterLvlComp = GetOwningPlayerPawn()->FindComponentByClass<UCharacterLevelComponent>();
	if (CharacterLvlComp && CharacterLvlComp->GetFreeStatsPoints() > 0)
	{
		CharacterLvlComp->AddStaminaPoints(1);
		CharacterLvlComp->AddFreeStatsPoints(-1);
		StaminaPoints->SetText(FText::FromString(FString::FromInt(CharacterLvlComp->GetStaminaPoints())));
		FreePointsCount->SetText(FText::FromString(FString::FromInt(CharacterLvlComp->GetFreeStatsPoints())));
	}
}

void UPlayerStatPoints::IncreaseStrengthPoints()
{
	auto CharacterLvlComp = GetOwningPlayerPawn()->FindComponentByClass<UCharacterLevelComponent>();
	if (CharacterLvlComp && CharacterLvlComp->GetFreeStatsPoints() > 0)
	{
		CharacterLvlComp->AddStrengthPoints(1);
		CharacterLvlComp->AddFreeStatsPoints(-1);
		StrengthPoints->SetText(FText::FromString(FString::FromInt(CharacterLvlComp->GetStrengthPoints())));
		FreePointsCount->SetText(FText::FromString(FString::FromInt(CharacterLvlComp->GetFreeStatsPoints())));
	}
}

void UPlayerStatPoints::IncreaseDexterityPoints()
{
	auto CharacterLvlComp = GetOwningPlayerPawn()->FindComponentByClass<UCharacterLevelComponent>();
	if (CharacterLvlComp && CharacterLvlComp->GetFreeStatsPoints() > 0)
	{
		CharacterLvlComp->AddDexterityPoints(1);
		CharacterLvlComp->AddFreeStatsPoints(-1);
		DexterityPoints->SetText(FText::FromString(FString::FromInt(CharacterLvlComp->GetDexterityPoints())));
		FreePointsCount->SetText(FText::FromString(FString::FromInt(CharacterLvlComp->GetFreeStatsPoints())));
	}
}

void UPlayerStatPoints::IncreaseIntelligencePoints()
{
	auto CharacterLvlComp = GetOwningPlayerPawn()->FindComponentByClass<UCharacterLevelComponent>();
	if (CharacterLvlComp && CharacterLvlComp->GetFreeStatsPoints() > 0)
	{
		CharacterLvlComp->AddIntelligencePoints(1);
		CharacterLvlComp->AddFreeStatsPoints(-1);
		IntelligencePoints->SetText(FText::FromString(FString::FromInt(CharacterLvlComp->GetIntelligencePoints())));
		FreePointsCount->SetText(FText::FromString(FString::FromInt(CharacterLvlComp->GetFreeStatsPoints())));
	}
}

void UPlayerStatPoints::SetFreePointsCount(int32 CurrentLevel)
{
	auto CharacterLvlComp = GetOwningPlayerPawn()->FindComponentByClass<UCharacterLevelComponent>();
	if (CharacterLvlComp)
	{
		FreePointsCount->SetText(FText::FromString(FString::FromInt(CharacterLvlComp->GetFreeStatsPoints())));
		//UpdateWidget();
	}
}

void UPlayerStatPoints::UpdateWidget()
{
	auto CharacterLvlComp = GetOwningPlayerPawn()->FindComponentByClass<UCharacterLevelComponent>();
	if (CharacterLvlComp)
	{
		HealthPoints->SetText(FText::FromString(FString::FromInt(CharacterLvlComp->GetHealthPoints())));
		StrengthPoints->SetText(FText::FromString(FString::FromInt(CharacterLvlComp->GetStrengthPoints())));
		StrengthPoints->SetText(FText::FromString(FString::FromInt(CharacterLvlComp->GetStrengthPoints())));
		DexterityPoints->SetText(FText::FromString(FString::FromInt(CharacterLvlComp->GetDexterityPoints())));
		IntelligencePoints->SetText(FText::FromString(FString::FromInt(CharacterLvlComp->GetIntelligencePoints())));
		FreePointsCount->SetText(FText::FromString(FString::FromInt(CharacterLvlComp->GetFreeStatsPoints())));
	}
}

