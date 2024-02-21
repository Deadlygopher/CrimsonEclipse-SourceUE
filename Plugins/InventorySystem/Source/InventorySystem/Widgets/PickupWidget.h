// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PickupWidget.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UPickupWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual bool Initialize() override;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PickupText;

public:
	void SetPickupText(FText TextToSet);
	
};
