// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInstance.h"
#include "Item.h"
//#include "InventoryComponent.h"
//#include "InventoryTypes.h"

UItemInstance::UItemInstance()
{

}

void UItemInstance::NativeOnConstruct()
{
	Size = Item->GetItemSize();
	bIsRotated = false;
	SizeInCells = Item->GetSizeInCells();
	ItemInstanceDamage = Item->GetWeaponDamage();
}

void UItemInstance::Rotate()
{
	if (!Item->CanBeRotated())
	{
		return;
	}

	if (bIsRotated)
	{
		Size = Item->GetItemSize();
		bIsRotated = false;
		SizeInCells = Item->GetSizeInCells();

		NotifyItemRotated();
		return;
	}

	SizeInCells.Empty();
	Size = FInvPoint2D(Item->GetItemSize().Y, Item->GetItemSize().X);

	for (const FInvPoint2D& Point: Item->GetSizeInCells())
	{
		SizeInCells.Add(FInvPoint2D(Point.Y, Point.X));
	}

	bIsRotated = true;
	NotifyItemRotated();
}

bool UItemInstance::IsRotated() const
{
	return bIsRotated;
}

void UItemInstance::ResetRotation()
{
	Size = Item->GetItemSize();
	bIsRotated = false;
	SizeInCells = Item->GetSizeInCells();
}

void UItemInstance::NotifyItemRotated()
{
	OnRotated();
	OnItemRotated.Broadcast();
}
