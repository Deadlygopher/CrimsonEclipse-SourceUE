// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

class UItemInstance;

UENUM(BlueprintType)
enum class EPickupState : uint8
{
	EWS_Initial UMETA(DisplayName = "Initial State"),
	EWS_Looted UMETA(DisplayName = "Looted"),
	EWS_Dropped UMETA(DisplayName = "Dropped"),

	EWS_MAX UMETA(DisplayName = "DefaultMax")
};

UCLASS(Blueprintable, BlueprintType)
class INVENTORYSYSTEM_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	

	APickup();
	
	virtual void BeginPlay() override;

	void OnPickupDataReceived() const;

	//virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = Pickup)
	void SetPickupData(UItemInstance* InItemInstance, int32 InQuantity);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnPickupDataReceived"), Category = "Pickup")
	void K2_OnPickupDataReceived();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
	UItemInstance* ItemInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
	int32 Quantity;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true), Category = "Pickup")
	UStaticMeshComponent* PickupMesh;

	//Initialize in open world items
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
	TSubclassOf<UItemInstance> InitItemInstance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
	int32 InitQuantity;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* SphereComponent;

	UFUNCTION(/*Client, Reliable*/)
	void SetWidgetVisibility(bool bVisible);

protected:
	UFUNCTION()
	virtual void OnOverlapComponentStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlapComponentEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pickup Widget")
	class UWidgetComponent* PickupWidget;


};
