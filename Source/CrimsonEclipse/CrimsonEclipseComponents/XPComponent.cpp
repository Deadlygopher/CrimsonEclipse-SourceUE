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
	//SetExpForNextLevel(CurrentLevel);
}

void UXPComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UXPComponent::ReceiveExp(int32 ExpToAdd)
{
	CurrentExp = FMath::Clamp(CurrentExp + ExpToAdd, 0, INT32_MAX);

	UE_LOG(LogXPComponent, Warning, TEXT("EXPtoAdd: %d"), ExpToAdd);

	while(CurrentExp>=NextLevelExp)
	{
		UE_LOG(LogXPComponent, Warning, TEXT("NEXT LEVEL"));

		++CurrentLevel;
		SetExpForNextLevel(CurrentLevel);
		OnLevelUp.Broadcast(CurrentLevel);
		//OnReceiveExp.Broadcast(CurrentLevelRequirment, CurrentExp - PrevLevelExp);

		//UE_LOG(LogXPComponent, Warning, TEXT("IncreaseXP, CurrentXP: %d"), CurrentExp - PrevLevelExp);
		//UE_LOG(LogXPComponent, Warning, TEXT("CUrrentLevelEXP: %d"), CurrentLevelRequirment);
	}
	OnReceiveExp.Broadcast(CurrentLevelRequirment, CurrentExp - PrevLevelExp);
}

void UXPComponent::SetExpForNextLevel(int32 NewLevel)
{
	/*
	if (NewLevel <= 1)
	{
		NextLevelExp = InitLevelExp;
		PrevLevelExp = 0;
		return;
	}*/

	NextLevelExpModifier = 1+RequireGrowCoefficient * NewLevel;
	NextLevelExp = NextLevelExpModifier* NextLevelExpModifier* NextLevelExpModifier * InitLevelExp * NewLevel;

	float PrevLevelExpModifier = 1+RequireGrowCoefficient * (NewLevel - 1);
	PrevLevelExp = PrevLevelExpModifier * PrevLevelExpModifier * PrevLevelExpModifier * InitLevelExp * (NewLevel - 1);

	CurrentLevelRequirment = NextLevelExp - PrevLevelExp;
}

void UXPComponent::SetCurrentLevel(int32 LevelToSet)
{
	CurrentLevel = LevelToSet;
	OnLevelUp.Broadcast(CurrentLevel);
	SetExpForNextLevel(CurrentLevel);
	OnReceiveExp.Broadcast(CurrentLevelRequirment, CurrentExp - PrevLevelExp);
}
