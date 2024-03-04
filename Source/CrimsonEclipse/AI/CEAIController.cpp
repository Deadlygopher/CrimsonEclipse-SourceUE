// Fill out your copyright notice in the Description page of Project Settings.


#include "CEAIController.h"
#include "CECharacterAI.h"
#include "Components/CEAIPerceptionComponent.h"


ACEAIController::ACEAIController()
{
	CEAIPerceptionComponent = CreateDefaultSubobject<UCEAIPerceptionComponent>("CEAIPerceptionComponent");
	SetPerceptionComponent(*CEAIPerceptionComponent);
}

void ACEAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const auto AimActor = CEAIPerceptionComponent->GetClosestEnemy();
	SetFocus(AimActor);
}

void ACEAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	const auto AICharacter = Cast<ACECharacterAI>(InPawn);
	if (AICharacter)
	{
		RunBehaviorTree(AICharacter->BehaviorTreeAsset);
	}
}
