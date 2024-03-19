// Fill out your copyright notice in the Description page of Project Settings.


#include "CrimsonEclipseAnimInstance.h"
#include "CrimsonEclipse/Character/CEBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCrimsonEclipseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	CrimsonEclipseCharacter = Cast<ACEBaseCharacter>(TryGetPawnOwner());
}


void UCrimsonEclipseAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	
	if (CrimsonEclipseCharacter == nullptr)
	{
		CrimsonEclipseCharacter = Cast<ACEBaseCharacter>(TryGetPawnOwner());
	}
	if (CrimsonEclipseCharacter == nullptr) return;

	FVector Velocity = CrimsonEclipseCharacter->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();
	Direction = CrimsonEclipseCharacter->GetMovementDirection();

	bIsRollig = CrimsonEclipseCharacter->GetIsRollPressed();
	bIsInAir = CrimsonEclipseCharacter->GetCharacterMovement()->IsFalling();
	bIsAccelerating = CrimsonEclipseCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
	bIsHitImpact = CrimsonEclipseCharacter->GetIsReceiveHitImpact();
}

