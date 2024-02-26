// Fill out your copyright notice in the Description page of Project Settings.


#include "CrimsonEclipseAnimInstance.h"
#include "CrimsonEclipse/Character/CrimsonEclipseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCrimsonEclipseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	CrimsonEclipseCharacter = Cast<ACrimsonEclipseCharacter>(TryGetPawnOwner());
}

void UCrimsonEclipseAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	
	if (CrimsonEclipseCharacter == nullptr)
	{
		CrimsonEclipseCharacter = Cast<ACrimsonEclipseCharacter>(TryGetPawnOwner());
	}
	if (CrimsonEclipseCharacter == nullptr) return;

	FVector Velocity = CrimsonEclipseCharacter->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();

	bIsInAir = CrimsonEclipseCharacter->GetCharacterMovement()->IsFalling();
	bIsAccelerating = CrimsonEclipseCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
}
