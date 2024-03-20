// Fill out your copyright notice in the Description page of Project Settings.


#include "CEAIController.h"
#include "CECharacterAI.h"
#include "Components/CEAIPerceptionComponent.h"
#include "BrainComponent.h"

#include "BehaviorTree/BlackboardComponent.h"


ACEAIController::ACEAIController()
{
	CEAIPerceptionComponent = CreateDefaultSubobject<UCEAIPerceptionComponent>("CEAIPerceptionComponent");
	SetPerceptionComponent(*CEAIPerceptionComponent);
}

void ACEAIController::SetWeaponTypeKey(bool bIsRangeWeapon)
{
	if (!GetBlackboardComponent()) return;
	GetBlackboardComponent()->SetValueAsBool(WeaponTypeKeyName, bIsRangeWeapon);
}

void ACEAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const auto AimActor = GetFocusOnActor();
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

void ACEAIController::OnUnPossess()
{
	Super::OnUnPossess();

	BrainComponent->Cleanup();
}

AActor* ACEAIController::GetFocusOnActor() const
{
	if(!GetBlackboardComponent()) return nullptr;
	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}
