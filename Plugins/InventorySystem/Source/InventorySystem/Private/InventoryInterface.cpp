// Fill out your copyright notice in the Description page of Project Settings.

//#include "Pickup.h"
#include "InventoryInterface.h"

// Add default functionality here for any IInventoryInterface functions that are not pure virtual.

bool IInventoryInterface::LootItem(APickup* Pickup, int32& LootedQuantity)
{
	return false;
}

void IInventoryInterface::OverlappingItemToArray(APickup* LastPickup)
{

}

void IInventoryInterface::RemoveItemFromOverlapping(APickup* NotPickup)
{

}

TArray<APickup*> IInventoryInterface::GetOverlappingItems()
{
	return TArray<APickup*>();
}

void IInventoryInterface::SwitchItemWidgetVisibility(bool bShowWidget)
{
}
