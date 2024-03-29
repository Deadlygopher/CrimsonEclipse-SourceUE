// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "InventoryComponent.h"
#include "Engine/DataAsset.h"

#include "Item.generated.h"

class UItemInstance;
class APickup;

/**
 * UItem
 */
UCLASS(BlueprintType, Blueprintable)
class INVENTORYSYSTEM_API UItem : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UItem();

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	UFUNCTION(BlueprintPure, Category = "Item")
	FString GetIdentifierString() const;

	UFUNCTION(BlueprintPure, Category = "Item")
	FString GetAssetName() const;

	UFUNCTION(BlueprintPure, Category = "Item")
	const FPrimaryAssetType& GetAssetType() const
	{
		return AssetType;
	}

	UFUNCTION(BlueprintPure, Category = "Item")
	TArray<FPoint2D> GetSizeInCells() const;

	UFUNCTION(BlueprintPure, Category = "Item")
	virtual bool CanBeRotated() const;

	UFUNCTION(BlueprintPure, Category = "Item")
	float GetScaledWeight() const;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Item")
	FPrimaryAssetType AssetType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Item")
	FText Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Item")
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Item")
	FSlateBrush Image;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Item")
	EItemType Type;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true, EditCondition = "Type == EItemType::OneHandWeapon || Type == EItemType::TwoHandsWeaponR || Type == EItemType::TwoHandsWeaponL"), Category = "Item")
	TSubclassOf<AActor> WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true, EditCondition = "Type == EItemType::OneHandWeapon || Type == EItemType::TwoHandsWeaponR || Type == EItemType::TwoHandsWeaponL"), Category = "Item")
	float Damage = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Item")
	TSubclassOf<UItemInstance> ItemInstanceClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Item")
	bool bCanBeDropped = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true, EditCondition = "bCanBeDropped"), Category = "Item")
	TSubclassOf<APickup> PickupClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Item")
	FString UnitPrefix;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Item")
	FString UnitSuffix;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Item")
	bool bCanBeStacked = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true, ClampMin = 1, UIMin = 1, EditCondition = "bCanBeStacked"), Category = "Item")
	int32 MaxStackSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Item")
	FPoint2D Size;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Item")
	bool bUseScaledWeight = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true, ClampMin = 0.0f, UIMin = 0.0f, EditCondition = "!bUseScaledWeight"), Category = "Item")
	float Weight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Item")
	bool bCanBeRotated = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true, EditCondition = "!bCanBeConsumed"), Category = "Item")
	bool bCanBeEquipped = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true, EditCondition = "bCanBeEquipped && !bCanBeConsumed"), Category = "Item")
	EEquipmentSlotType PrimaryEquipmentSlot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true, EditCondition = "bCanBeEquipped && !bCanBeConsumed"), Category = "Item")
	EEquipmentSlotType SecondaryEquipmentSlot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true, EditCondition = "!bCanBeEquipped"), Category = "Item")
	bool bCanBeConsumed = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true, EditCondition = "bCanBeConsumed", ClampMin = 1, UIMin = 1), Category = "Item")
	int32 ConsumedQuantityPerUsage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Item")
	UStaticMesh* PickupStaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Item")
	FVector PickupStaticMeshScale;


public:
	UFUNCTION(BlueprintCallable)
	FText GetItemName() const { return Name; }

	UFUNCTION(BlueprintCallable)
	FText GetItemDescription() const { return Description; }

	UFUNCTION(BlueprintCallable)
	FSlateBrush GetItemImage() const { return Image; }

	UFUNCTION(BlueprintCallable)
	EItemType GetItemType() const { return Type; }

	UFUNCTION(BlueprintCallable)
	TSubclassOf<UItemInstance> GetItemInstanceClass() const { return ItemInstanceClass; }

	UFUNCTION(BlueprintCallable)
	bool GetItemCanBeDropped() const { return bCanBeDropped; }

	UFUNCTION(BlueprintCallable)
	TSubclassOf<APickup> GetItemPickupClass() const { return PickupClass; }

	UFUNCTION(BlueprintCallable)
	FString GetUnitPrefix() const { return UnitPrefix; }

	UFUNCTION(BlueprintCallable)
	FString GetUnitSuffix() const { return UnitSuffix; }

	UFUNCTION(BlueprintCallable)
	bool GetItemCanBeStacked() const { return bCanBeStacked; }

	UFUNCTION(BlueprintCallable)
	int32 GetItemMaxStackSize() const { return MaxStackSize; }

	UFUNCTION(BlueprintCallable)
	FPoint2D GetItemSize() const { return Size; }

	UFUNCTION(BlueprintCallable)
	bool GetItemUseScaledWeight() const { return bUseScaledWeight; }

	UFUNCTION(BlueprintCallable)
	float GetItemWeight() const { return Weight; }

	//UFUNCTION(BlueprintCallable)
	//bool GetItemCanBeRotated() const { return bCanBeRotated; }

	UFUNCTION(BlueprintCallable)
	bool GetItemCanBeEquipped() const { return bCanBeEquipped; }

	UFUNCTION(BlueprintCallable)
	EEquipmentSlotType GetItemPrimaryEquipmentSlotType() const { return PrimaryEquipmentSlot; }

	UFUNCTION(BlueprintCallable)
	EEquipmentSlotType GetItemSecondaryEquipmentSlotType() const { return SecondaryEquipmentSlot; }

	UFUNCTION(BlueprintCallable)
	bool GetItemCanBeConsumed() const { return bCanBeConsumed; }

	UFUNCTION(BlueprintCallable)
	int32 GetItemConsumedQuantityPerUsage() const { return ConsumedQuantityPerUsage; }

	UFUNCTION(BlueprintCallable)
	UStaticMesh* GetItemPickupStaticMesh() const { return PickupStaticMesh; }

	UFUNCTION(BlueprintCallable)
	FVector GetItemPickupStaticMeshScale() const { return PickupStaticMeshScale; }

	UFUNCTION(BlueprintCallable)
	TSubclassOf<AActor> GetWeaponType() const { return WeaponType; };

	UFUNCTION(BlueprintCallable)
	float GetWeaponDamage() const { return Damage; }
};
