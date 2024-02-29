// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "CrimsonEclipsePlayerController.generated.h"

UCLASS()
class ACrimsonEclipsePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACrimsonEclipsePlayerController();

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Resets HMD orientation in VR. */
	//void OnResetVR();

	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();

	/** Navigate player to the current touch location. */
	void MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location);
	
	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

	/** Common request to nagivate player to the given world location. */
	void RequestSetNewMoveDestination(const FVector DestLocation);

	/** Call navigate player to the given world location (Client Version). */
	UFUNCTION(Reliable, Client)
	void ClientSetNewMoveDestination(const FVector DestLocation);

	/** Call navigate player to the given world location (Server Version). */
	UFUNCTION(Reliable, Server)
	void ServerSetNewMoveDestination(const FVector DestLocation);

	/** Navigate player to the given world location. 
	void SetNewMoveDestination(const FVector DestLocation);*/

	UFUNCTION(BlueprintCallable)
	void PickupItem();

	UFUNCTION(BlueprintCallable)
	void ShowPickupWidget();

	UFUNCTION(Server, Reliable)
	void ServerPickupButtonPressed();

	void PickupButtonPressed();


private:
	bool bShowWidget = false;
};


