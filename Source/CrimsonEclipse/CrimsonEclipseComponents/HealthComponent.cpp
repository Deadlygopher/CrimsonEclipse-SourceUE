// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Net/UnrealNetwork.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHealthComponent::SetMaxHealth(float NewMaxHealth)
{
	//if (IsDead()) return;
	if (NewMaxHealth > 0) MaxHealth = NewMaxHealth;
	OnHealthChange.Broadcast(Health, MaxHealth);
}

void UHealthComponent::IncreaseHealth(float HealthToIncrease, AActor* DamageCauser)
{
	if (IsDead()) return;
	Health = FMath::Clamp(Health + HealthToIncrease, 0.f, MaxHealth);
	OnHealthChange.Broadcast(Health, MaxHealth);

	if (IsDead())
	{
		OnDeath.Broadcast(DamageCauser);
	}
}

void UHealthComponent::DecreaseHealth(float HealthToDecrease, AActor* DamageCauser)
{
	if (IsDead()) return;
	Health = FMath::Clamp(Health - HealthToDecrease, 0.f, MaxHealth);
	OnHealthChange.Broadcast(Health, MaxHealth);

	if (IsDead())
	{
		OnDeath.Broadcast(DamageCauser);
	}
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UHealthComponent, Health, COND_None);
	DOREPLIFETIME_CONDITION(UHealthComponent, MaxHealth, COND_None);
}
