// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CEBaseCharacter.h"
#include "CrimsonEclipse/Interfaces/HitDetectInterface.h"

#include "CrimsonEclipseCharacter.generated.h"

class UXPComponent;

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

	UPROPERTY(VisibleAnywhere)
	UXPComponent* XPComponent;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;

	bool bIsCameraRotate = false;


	/// SHOW / HIDE OVERHEAD WIDGET REPLICATION ///
	UFUNCTION()
	void ShowHideSelfHealthOverhead();

	UFUNCTION(Client, Unreliable)
	void Client_ShowHideSelfHealthOverhead();

	UFUNCTION(Server, Unreliable)
	void Server_ShowHideSelfHealthOverhead();
	/// SHOW / HIDE OVERHEAD WIDGET REPLICATION ///


protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	void CameraRotateON();
	void CameraRotateOFF();
	void ChangeCameraAngle(float Value);
	void CameraZoom(float Value);


	/// ON HEALTH CHANGE REPLICATE ///
	virtual void SetHealthWidgetInfo(float NewHealth, float MaxHealth) override;

	UFUNCTION(Client, Reliable)
	void Client_SetHealthWidgetInfo(float NewHealth, float MaxHealth);
	/// ON HEALTH CHANGE REPLICATE ///


	UFUNCTION(BlueprintCallable)
	void SetExpProgressHUD(int32 CurrentLevelMaxExp, int32 CurrentLevelExp);
	UFUNCTION(BlueprintCallable)
	void SetNewLevelHUD(int32 NewLevel);
	UFUNCTION(BlueprintCallable)
	void SetNewLevelForLvlComp(int32 NewLevel);

public:
	virtual void StartRoll() override;
	virtual void RequestLightAttack() override;

	
	/// ROTATE TO CURSOR IMPLEMENTATION ///
	virtual void RotateToCursorDirecion() override;

	UFUNCTION(Server, Reliable)
	void Server_RotateToCursorDirecion(FRotator NewRotation);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_RotateToCursorDirecion(FRotator NewRotation);
	/// ROTATE TO CURSOR IMPLEMENTATION ///

	void OnDeath(AActor* DamageCauser) override;
};
