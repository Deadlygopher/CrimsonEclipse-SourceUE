// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "InventoryComponent.h"
#include "UObject/NoExportTypes.h"
#include "InventoryTypes.h"
#include "ItemInstance.generated.h"

class UItem;
class UInventoryComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FItemEvent);


UCLASS(Abstract, BlueprintType, Blueprintable)
class INVENTORYSYSTEM_API UItemInstance : public UObject
{
	GENERATED_BODY()

public:

	UItemInstance();

	void NativeOnConstruct();

	UFUNCTION(BlueprintImplementableEvent, Category = "ItemInstance")
	void OnConstruct();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "ItemInstance")
	void OnRotated();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "ItemInstance")
	void OnUsed();

	UFUNCTION(BlueprintCallable, Category = "ItemInstance")
	void Rotate();

	UFUNCTION(BlueprintPure, Category = "ItemInstance")
	bool IsRotated() const;

	UFUNCTION(BlueprintCallable, Category = "ItemInstance")
	void ResetRotation();
	
	UFUNCTION(BlueprintCallable, Category = "ItemInstance")
	void GenerateRarity();

	void NotifyItemRotated();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "ItemInstance")
	UItem* Item;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "ItemInstance")
	FInvPoint2D TopLeftCoordinates;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "ItemInstance")
	TArray<FInvPoint2D> SizeInCells;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "ItemInstance")
	FInvPoint2D Size;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "ItemInstance")
	uint8 bIsRotated : 1;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "ItemInstance")
	UInventoryComponent* OwnerInventory;

	UPROPERTY(BlueprintAssignable)
	FItemEvent OnItemRotated;

	UPROPERTY(BlueprintReadWrite)
	float ItemInstanceDamage = 0;
	
	UPROPERTY(BlueprintReadWrite)
	EItemRarity ItemRarity;
};
