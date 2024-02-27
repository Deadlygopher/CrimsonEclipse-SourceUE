// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CEBaseCharacter.generated.h"

class UCombatComponent;
class UHealthComponent;
class UWidgetComponent;
//class AController;

UCLASS()
class CRIMSONECLIPSE_API ACEBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACEBaseCharacter();
	virtual void PostInitializeComponents() override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCombatComponent* CombatComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* OverheadWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UHealthComponent* HealthComponent;

	virtual void SetOverheadWidgetInfo(float NewHealth, float MaxHealth);

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
		AController* InstigatorController, AActor* DamageCauser);
};