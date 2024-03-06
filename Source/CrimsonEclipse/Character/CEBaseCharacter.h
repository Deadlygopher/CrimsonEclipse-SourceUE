// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CrimsonEclipse/Interfaces/EvadeAnimInterface.h"

#include "CEBaseCharacter.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCEBaseCharacter, Log, All);

class UCombatComponent;
class UHealthComponent;
class UWidgetComponent;
class UInventoryComponent;

UCLASS()
class CRIMSONECLIPSE_API ACEBaseCharacter : public ACharacter, public IEvadeAnimInterface
{
	GENERATED_BODY()

public:
	ACEBaseCharacter();
	virtual void PostInitializeComponents() override;

	virtual void LightAttack();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	float GetMaxWalkSpeed();

	UHealthComponent* GetHealthComponent();
	float GetHealth() const;
	float GetMaxHealth() const;

	virtual void RotateToCursorDirecion();

	/// Roll variables
private:
	UPROPERTY(EditDefaultsOnly)
	bool bPressedRoll = false;

	UPROPERTY(EditDefaultsOnly)
	float RollSpeed = 1000;

public:
	bool GetIsRollPressed() { return bPressedRoll; }

	UFUNCTION(BlueprintCallable)
	virtual void StartRoll();

	virtual void RollInProcess() override;
	virtual void StopRoll() override;
	/// Roll variables

	/// Inventory
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UInventoryComponent* InventoryComponent;

	UFUNCTION(BlueprintCallable)
	void OnItemEquip(class UItem* InItem, EEquipmentSlotType Type, int32 InQuantity);

	UFUNCTION(BlueprintCallable)
	void OnItemUnequip(class UItem* InItem, EEquipmentSlotType Type, int32 InQuantity);
	/// Inventory

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCombatComponent* CombatComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* OverheadWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float MaxMoveSpeed = 600;

	virtual void SetHealthWidgetInfo(float NewHealth, float MaxHealth);

private:
	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
		AController* InstigatorController, AActor* DamageCauser);

	bool bReadyForAttack = true;

	UFUNCTION()
	void ResetReadyForAttack(UAnimMontage* Montage, bool bInterrupted);
};