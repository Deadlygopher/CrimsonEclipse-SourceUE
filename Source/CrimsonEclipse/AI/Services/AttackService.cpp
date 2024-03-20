// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackService.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CrimsonEclipse/CrimsonEclipseComponents/CombatComponent.h"

UAttackService::UAttackService()
{
	NodeName = "Attack Service";
}

void UAttackService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Controller = OwnerComp.GetAIOwner();
	const auto Blackboard = OwnerComp.GetBlackboardComponent();

	const auto HasAim = Blackboard && Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName);

	if (Controller)
	{
		const auto CombatComponent = Controller->GetPawn()->FindComponentByClass<UCombatComponent>();
		if (CombatComponent && HasAim)
		{
			CombatComponent->LightAttack();
		}
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
