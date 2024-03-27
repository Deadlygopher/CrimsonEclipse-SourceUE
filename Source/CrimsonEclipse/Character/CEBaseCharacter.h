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
class USphereComponent;
class UItem;
class UItemInstance;
class UCharacterLevelComponent;

UCLASS()
class CRIMSONECLIPSE_API ACEBaseCharacter : public ACharacter, public IEvadeAnimInterface
{
	GENERATED_BODY()

public:
	ACEBaseCharacter();
	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintCallable)
	virtual void RequestLightAttack();

	UFUNCTION(BlueprintCallable)
	virtual void RequestHeavyAttack();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	float GetMovementDirection() const;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	float GetMaxWalkSpeed() const;
	void SetMaxWalkSpeed(float NewSpeed);

	UHealthComponent* GetHealthComponent();
	float GetHealth() const;
	float GetMaxHealth() const;

	virtual void RotateToCursorDirecion();
	virtual bool GetIsReceiveHitImpact() const { return bIsReceiveHitImpact; }
	void SetAttackRadius(float RadiusForSet);

	void SetIsRangeWeapon(bool bIsRange) { bIsRangeWeapon = bIsRange; }
	bool GetIsRangeWeapon() {return bIsRangeWeapon;	};
	bool bIsRangeWeapon = false;

	/// Roll variables
private:
	UPROPERTY(EditDefaultsOnly)
	bool bPressedRoll = false;

	UPROPERTY(EditDefaultsOnly)
	float RollSpeed = 1000;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* DeathAnimation;

public:
	bool GetIsRollPressed() const { return bPressedRoll; }

	UFUNCTION(BlueprintCallable)
	virtual void StartRoll();

	virtual void RollInProcess() override;
	virtual void StopRoll() override;
	/// Roll variables

	/// Inventory
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UInventoryComponent* InventoryComponent;

	UFUNCTION(BlueprintCallable)
	virtual void OnItemEquip(UItem* InItem, UItemInstance* InIntemInstance, EEquipmentSlotType Type, int32 InQuantity);

	UFUNCTION(BlueprintCallable)
	void OnItemUnequip(UItem* InItem, UItemInstance* InIntemInstance, EEquipmentSlotType Type, int32 InQuantity);
	/// Inventory

	void OnClickAttack();
	void SetTargetActor(APawn* TActor) { TargetActor = TActor; };

	bool GetReadyForAttack() const { return bReadyForAttack; }
	void SetReadyForAttack(bool bIsReady) { bReadyForAttack = bIsReady; }


protected:
	virtual void BeginPlay() override;

	virtual void OnDeath(AActor* DamageCauser);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCombatComponent* CombatComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* OverheadWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere)
	UCharacterLevelComponent* LevelComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float MaxMoveSpeed = 600;

	virtual void SetHealthWidgetInfo(float NewHealth, float MaxHealth);

private:
	UFUNCTION(BlueprintCallable)
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
		AController* InstigatorController, AActor* DamageCauser);

	bool bReadyForAttack = true;

	//UFUNCTION()
	void AfterDeathAnimation();

	UFUNCTION()
	void ResetReadyForAttack(UAnimMontage* Montage, bool bInterrupted);

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* AttackReachRadius;

	UFUNCTION()
	void OnReachAttackRadius(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY()
	APawn* TargetActor = nullptr;

	UPROPERTY(EditDefaultsOnly)
	float AttackRadius = 150.f;

	//bool bWeaponRadiusReached = false;
	bool bAttackClicked = false;

	bool bIsReceiveHitImpact;
	//UFUNCTION()
	void IsReceiveHitImpactReset();
};