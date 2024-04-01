// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterLevelComponent.h"
#include "HealthComponent.h"

UCharacterLevelComponent::UCharacterLevelComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharacterLevelComponent::UpdateAllStats()
{
	UpdateHealthPointsDependencies();
}

void UCharacterLevelComponent::CountFreeStatsPoints()
{
	int32 AllStatSummInit = InitHealthPoints + InitStaminaPoints + InitStrengthPoints + InitDexterityPoints + InitIntelligencePoints;
	int32 AllStatSummCurrent = HealthPoints + StaminaPoints + StrengthPoints + DexterityPoints + IntelligencePoints;
	FreeStatsPoints += AllStatSummCurrent - AllStatSummInit + FreeStatsPointsPerLevel * (CurrentLevel-1);
}

void UCharacterLevelComponent::BeginPlay()
{
	Super::BeginPlay();
	CountCurrentExpForKill();

	CountFreeStatsPoints();
}

void UCharacterLevelComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCharacterLevelComponent::LevelUp()
{
	++CurrentLevel;
	AddFreeStatsPoints(FreeStatsPointsPerLevel);
}

void UCharacterLevelComponent::CountCurrentExpForKill()
{
	CurrentExpForKill = InitExpForKill + InitExpForKill * 0.2 * CurrentLevel;
}


void UCharacterLevelComponent::AddHealthPoints(int32 NewHealthPoints)
{
	HealthPoints += NewHealthPoints;
	UpdateHealthPointsDependencies();
}

void UCharacterLevelComponent::UpdateHealthPointsDependencies()
{
	auto HealthComp = GetOwner()->FindComponentByClass<UHealthComponent>();
	if (HealthComp)
	{
		auto NewMaxHealth = HealthComp->GetInitMaxHealth() + HealthComp->GetInitMaxHealth() * 0.1 * HealthPoints;
		HealthComp->SetMaxHealth(NewMaxHealth);
	}
}
