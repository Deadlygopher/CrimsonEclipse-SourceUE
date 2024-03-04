// Fill out your copyright notice in the Description page of Project Settings.


#include "CEAIPerceptionComponent.h"
#include "AIController.h"
#include "CrimsonEclipse/CrimsonEclipseComponents/HealthComponent.h"
#include "Perception/AISense_Sight.h"
//#include "Perception/AIPerceptionTypes.h"


AActor* UCEAIPerceptionComponent::GetClosestEnemy() const
{
	TArray<AActor*> PerceivedActors;
	GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);
	if (PerceivedActors.Num() == 0) return nullptr;

	const auto Controller = Cast<AAIController>(GetOwner());
	if (!Controller) return nullptr;

	const auto Pawn = Controller->GetPawn();
	if (!Pawn) return nullptr;

	float BestDistance = MAX_FLT;
	AActor* BestDistancePawn = nullptr;
	for (auto PerceivedActor : PerceivedActors)
	{
		const auto HealthComponent = Cast<UHealthComponent>(PerceivedActor->FindComponentByClass(UHealthComponent::StaticClass()));
		if (HealthComponent && !HealthComponent->IsDead()) // TODO check if enemies
		{
			const auto CurrentDistance = Pawn->GetDistanceTo(PerceivedActor);
			if (CurrentDistance < BestDistance)
			{
				BestDistance = CurrentDistance;
				BestDistancePawn = PerceivedActor;
			}
		}
	}
	return BestDistancePawn;
}
