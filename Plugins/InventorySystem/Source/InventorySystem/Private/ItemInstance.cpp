// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInstance.h"
#include "Item.h"
#include "InventoryComponent.h"
#include "Net/UnrealNetwork.h"
//#include "Kismet/KismetMathLibrary.h"

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



void UItemInstance::GenerateRarity()
{
	auto RarityChance = FMath::RandHelper(100);
	UE_LOG(LogTemp, Warning, TEXT("RARITY: %d"), RarityChance);

	if (RarityChance >= 99)
	{
		ItemRarity = EItemRarity::EIR_Legendary;
		return;
	}
	else if (RarityChance >= 95)
	{
		ItemRarity = EItemRarity::EIR_Epic;
		return;
	}
	else if (RarityChance >= 85)
	{
		ItemRarity = EItemRarity::EIR_Rare;
		return;
	}
	else if (RarityChance >= 70)
	{
		ItemRarity = EItemRarity::EIR_Uncommon;
		return;
	}
	else if (RarityChance >= 0)
	{
		ItemRarity = EItemRarity::EIR_Common;
		return;
	}
}

void UItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UItemInstance, ItemRarity, COND_None);
}