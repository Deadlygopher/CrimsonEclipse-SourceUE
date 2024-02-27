// Fill out your copyright notice in the Description page of Project Settings.


#include "CEBaseCharacter.h"
#include "CrimsonEclipse/CrimsonEclipseComponents/CombatComponent.h"
#include "CrimsonEclipse/CrimsonEclipseComponents/HealthComponent.h"
#include "Components/WidgetComponent.h"
#include "CrimsonEclipse/HUD/OverheadWidget.h"
//#include "GameFramework/Controller.h"


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

	SetOverheadWidgetInfo(HealthComponent->GetHealth(), HealthComponent->GetMaxHealth());

	HealthComponent->OnHealthChange.AddUObject(this, &ACEBaseCharacter::SetOverheadWidgetInfo);
	if (IsPlayerControlled())
	{
		OverheadWidgetComponent->SetVisibility(false);
	}
	OnTakeAnyDamage.AddDynamic(this, &ACEBaseCharacter::ReceiveDamage);
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

void ACEBaseCharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatorController, AActor* DamageCauser)
{
	HealthComponent->DecreaseHealth(Damage);
	UE_LOG(LogTemp, Warning, TEXT("DELEGATE"));
}

void ACEBaseCharacter::SetOverheadWidgetInfo(float NewHealth, float MaxHealth)
{
	if (OverheadWidgetComponent)
	{
		auto Widget = Cast<UOverheadWidget>(OverheadWidgetComponent->GetWidget());
		//float MaxHealth = HealthComponent->GetMaxHealth();
		Widget->UpdateHealthBar(NewHealth, MaxHealth);
	}
}

