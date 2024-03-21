// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CrimsonEclipse/Character/CEBaseCharacter.h"
#include "CECharacterAI.generated.h"

class UBehaviorTree;

UCLASS()
class CRIMSONECLIPSE_API ACECharacterAI : public ACEBaseCharacter
{
	GENERATED_BODY()
	
public:
	ACECharacterAI();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
	UBehaviorTree* BehaviorTreeAsset;

	//UFUNCTION(BlueprintCallable)
	void OnItemEquip(class UItem* InItem, class UItemInstance* InItemInstance, EEquipmentSlotType Type, int32 InQuantity) override;
};