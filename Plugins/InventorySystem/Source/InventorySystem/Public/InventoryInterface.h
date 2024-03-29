// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInventoryInterface : public UInterface
{
	GENERATED_BODY()
};


class INVENTORYSYSTEM_API IInventoryInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool LootItem(class APickup* Pickup, int32& LootedQuantity);

	virtual void OverlappingItemToArray(APickup* LastPickup);

	virtual void RemoveItemFromOverlapping(APickup* NotItem);

	virtual TArray<APickup*> GetOverlappingItems();

	virtual void SwitchItemWidgetVisibility(bool bShowWidget);
};
