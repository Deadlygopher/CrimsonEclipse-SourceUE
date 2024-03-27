// Fill out your copyright notice in the Description page of Project Settings.

#include "XPComponent.h"

DEFINE_LOG_CATEGORY(LogXPComponent);

UXPComponent::UXPComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UXPComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UXPComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UXPComponent::ReceiveExp(int32 ExpToAdd)
{
	CurrentExp = FMath::Clamp(CurrentExp + ExpToAdd, 0, INT32_MAX);

	for(CurrentExp;CurrentExp>=NextLevelExp;)
	{
		UE_LOG(LogXPComponent, Warning, TEXT("NEXT LEVEL"));
		PrevLevelExp = NextLevelExp;
		SetExpForNextLevel(1);
		CurrentLevelRequirment = NextLevelExp - PrevLevelExp;
		UE_LOG(LogXPComponent, Warning, TEXT("IncreaseXP, CurrentXP: %d"), CurrentExp - PrevLevelExp);
		UE_LOG(LogXPComponent, Warning, TEXT("CUrrentLevelEXP: %d"), CurrentLevelRequirment);
	}
}

void UXPComponent::SetExpForNextLevel(int32 NewLevel)
{
	NextLevelExpModifier += RequireGrowCoefficient * 2;//NewLevel;
	NextLevelExp += NextLevelExp * NextLevelExpModifier;
}
