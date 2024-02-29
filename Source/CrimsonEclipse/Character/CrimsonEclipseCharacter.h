// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CEBaseCharacter.h"
#include "CrimsonEclipse/Interfaces/HitDetectInterface.h"

#include "CrimsonEclipseCharacter.generated.h"


UCLASS(Blueprintable)
class ACrimsonEclipseCharacter : public ACEBaseCharacter
{
	GENERATED_BODY()

public:
	ACrimsonEclipseCharacter();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//virtual void PostInitializeComponents() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;

	bool bIsCameraRotate = false;

	UFUNCTION(Server, Reliable)
	void ServerEquipWeapon(UItem* InItem, EEquipmentSlotType Type, int32 InQuantity);


public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)

	class UInventoryComponent* InventoryComponent;

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	void CameraRotateON();
	void CameraRotateOFF();
	void ChangeCameraAngle(float Value);
	void CameraZoom(float Value);

	UFUNCTION()
	void EquipWeapon(UItem* InItem, EEquipmentSlotType Type, int32 InQuantity);

	virtual void SetOverheadWidgetInfo(float NewHealth, float MaxHealth) override;

public:
	UFUNCTION(BlueprintCallable)
	void OnItemEquip(class UItem* InItem, EEquipmentSlotType Type, int32 InQuantity);

	UFUNCTION(BlueprintCallable)
	void OnItemUnequip(class UItem* InItem, EEquipmentSlotType Type, int32 InQuantity);
};
