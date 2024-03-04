// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHealthComponent::IncreaseHealth(float HealthToIncrease)
{
	if (IsDead()) return;
	Health = FMath::Clamp(Health + HealthToIncrease, 0.f, MaxHealth);
	OnHealthChange.Broadcast(Health, MaxHealth);

	if (IsDead())
	{
		OnDeath.Broadcast();
	}
}

void UHealthComponent::DecreaseHealth(float HealthToDecrease)
{
	if (IsDead()) return;
	Health = FMath::Clamp(Health - HealthToDecrease, 0.f, MaxHealth);
	OnHealthChange.Broadcast(Health, MaxHealth);

	if (IsDead())
	{
		OnDeath.Broadcast();
	}
}