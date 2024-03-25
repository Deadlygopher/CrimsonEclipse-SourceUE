// Fill out your copyright notice in the Description page of Project Settings.


#include "XPComponent.h"

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

void UXPComponent::IncreaseCurrentXP(int32 ExpToAdd)
{
	CurrentExp = FMath::Clamp(CurrentExp + ExpToAdd, 0, INT32_MAX);
}

void UXPComponent::SetExpForNextLevel(int32 NewLevel)
{
	NextLevelExpModifier += RequirGrowCoefficient * NewLevel;
	NextLevelExp = NextLevelExp * NextLevelExpModifier;
}
