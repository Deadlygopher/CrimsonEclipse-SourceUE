// Fill out your copyright notice in the Description page of Project Settings.


#include "CEBaseCharacter.h"
#include "CrimsonEclipse/CrimsonEclipseComponents/CombatComponent.h"
#include "CrimsonEclipse/CrimsonEclipseComponents/HealthComponent.h"
#include "Components/WidgetComponent.h"
#include "CrimsonEclipse/HUD/OverheadWidget.h"


ACEBaseCharacter::ACEBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	CombatComponent->SetIsReplicated(true);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	OverheadWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidgetComponent->SetupAttachment(RootComponent);
}

void ACEBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetOverheadWidgetInfo(HealthComponent->GetHealth());

	HealthComponent->OnHealthChange.AddUObject(this, &ACEBaseCharacter::SetOverheadWidgetInfo);
}

void ACEBaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (CombatComponent)
	{
		CombatComponent->SetCharacter(this);
	}
}

void ACEBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACEBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACEBaseCharacter::SetOverheadWidgetInfo(float NewHealth)
{
	if (OverheadWidgetComponent)
	{
		auto Widget = Cast<UOverheadWidget>(OverheadWidgetComponent->GetWidget());
		//float Health = HealthComponent->GetHealth();
		float MaxHealth = HealthComponent->GetMaxHealth();
		Widget->UpdateHealthBar(NewHealth, MaxHealth);
		UE_LOG(LogTemp, Warning, TEXT("DELEGATE"));
	}
}

