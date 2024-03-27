// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterLevelComponent.h"

UCharacterLevelComponent::UCharacterLevelComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharacterLevelComponent::BeginPlay()
{
	Super::BeginPlay();
	CountCurrentExpForKill();
}

void UCharacterLevelComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCharacterLevelComponent::CountCurrentExpForKill()
{
	CurrentExpForKill = InitExpForKill + InitExpForKill * 0.2 * CurrentLevel;
}