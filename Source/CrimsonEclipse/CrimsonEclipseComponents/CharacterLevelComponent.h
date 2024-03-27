// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterLevelComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRIMSONECLIPSE_API UCharacterLevelComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCharacterLevelComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	int32 GetCurrentExpForKill() const { return CurrentExpForKill; }

	int32 GetCurrentLevel() const { return CurrentLevel; }

	void CountCurrentExpForKill();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 1))
	int32 CurrentLevel = 1;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 1))
	int32 InitExpForKill = 1;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 1))
	int32 CurrentExpForKill = 1;
};
