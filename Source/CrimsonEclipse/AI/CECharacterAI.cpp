// Fill out your copyright notice in the Description page of Project Settings.


#include "CECharacterAI.h"
#include "CEAIController.h"
#include "GameFramework/CharacterMovementComponent.h"


ACECharacterAI::ACECharacterAI()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = ACEAIController::StaticClass();

	bUseControllerRotationYaw = false;
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);  // TODO Magic Number
	}
}
