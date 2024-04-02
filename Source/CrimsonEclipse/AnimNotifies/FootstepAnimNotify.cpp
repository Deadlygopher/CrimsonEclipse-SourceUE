// Fill out your copyright notice in the Description page of Project Settings.


#include "FootstepAnimNotify.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

#include "Kismet/KismetSystemLibrary.h"

void UFootstepAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	auto Character = MeshComp->GetOwner();
	if (!Character) return;
	FHitResult FootHit;
	FCollisionQueryParams Params;
	
	TArray<AActor*> ActorsIgnore;
	ActorsIgnore.Add(Character);
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), Character->GetActorLocation(),
		FVector(Character->GetActorLocation().X, Character->GetActorLocation().Y, -100), UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false, ActorsIgnore, EDrawDebugTrace::None, FootHit, true/*, FLinearColor::Blue, FLinearColor::Red*/);

	if (FootHit.PhysMaterial.Get())
	{
		switch (FootHit.PhysMaterial->SurfaceType)
		{
		case EPhysicalSurface::SurfaceType_Default:
		{
			if (DefaultSound) UGameplayStatics::PlaySoundAtLocation(this, DefaultSound, FootHit.Location);
			break;
		}

		default:
		{
			break;
		}
		}
	}
}