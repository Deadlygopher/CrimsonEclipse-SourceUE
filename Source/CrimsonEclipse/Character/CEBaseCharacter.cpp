// Fill out your copyright notice in the Description page of Project Settings.


#include "CEBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CrimsonEclipse/CrimsonEclipseComponents/CombatComponent.h"
#include "CrimsonEclipse/CrimsonEclipseComponents/HealthComponent.h"
#include "Components/WidgetComponent.h"
#include "CrimsonEclipse/HUD/OverheadWidget.h"
#include "CrimsonEclipse/Items/Weapon.h"
#include "Kismet/KismetMathLibrary.h"
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

	GetCharacterMovement()->MaxWalkSpeed = MaxMoveSpeed;
	//GetMovementComponent()->
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

void ACEBaseCharacter::LightAttack()
{
	if (CombatComponent->GetRightHandWeapon() && bReadyForAttack)
	{
		RotateToCursorDirecion();
		GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &ACEBaseCharacter::ResetReadyForAttack);
		auto AnimMontage = CombatComponent->GetRightHandWeapon()->GetLightAttackAnimMontage();
		if (AnimMontage)
		{
			bReadyForAttack = false;
			GetCharacterMovement()->MaxWalkSpeed = InAttackMoveSpeed;
			GetMesh()->GetAnimInstance()->Montage_Play(AnimMontage);
		}
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

void ACEBaseCharacter::StartRoll()
{
	RotateToCursorDirecion();
	bReadyForAttack = false;
	GetCharacterMovement()->MaxWalkSpeed = RollSpeed;
	GetCharacterMovement()->Velocity = GetActorForwardVector() * RollSpeed;
	bPressedRoll = true;
}

void ACEBaseCharacter::RollInProcess()
{
	GetCharacterMovement()->Velocity = GetActorForwardVector() * RollSpeed;
}

void ACEBaseCharacter::StopRoll()
{
	GetCharacterMovement()->MaxWalkSpeed = MaxMoveSpeed;
	bPressedRoll = false;
	bReadyForAttack = true;
}

void ACEBaseCharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatorController, AActor* DamageCauser)
{
	HealthComponent->DecreaseHealth(Damage);
	UE_LOG(LogTemp, Warning, TEXT("DELEGATE"));
}

void ACEBaseCharacter::ResetReadyForAttack(UAnimMontage* Montage, bool bInterrupted)
{
	bReadyForAttack = true;
	GetCharacterMovement()->MaxWalkSpeed = MaxMoveSpeed;
}

void ACEBaseCharacter::RotateToCursorDirecion()
{
	FHitResult HitResult;
	Cast<APlayerController>(GetController())->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
	FVector LastLocation = GetCharacterMovement()->GetLastUpdateLocation();
	FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(LastLocation, HitResult.Location);
	GetCharacterMovement()->MoveUpdatedComponent(FVector{ 0 }, FRotator{ 0.f,NewRotation.Yaw, NewRotation.Roll }, true);
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

