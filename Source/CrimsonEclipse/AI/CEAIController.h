// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CEAIController.generated.h"

class UCEAIPerceptionComponent;

UCLASS()
class CRIMSONECLIPSE_API ACEAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ACEAIController();

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCEAIPerceptionComponent* CEAIPerceptionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FName FocusOnKeyName = "EnemyActor";

private:
	AActor* GetFocusOnActor() const;
};
