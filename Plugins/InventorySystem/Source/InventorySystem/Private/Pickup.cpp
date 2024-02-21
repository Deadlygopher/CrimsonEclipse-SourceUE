// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickup.h"
#include "InventoryInterface.h"
#include "ItemInstance.h"
#include "Item.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "InventorySystem/Widgets/PickupWidget.h"


APickup::APickup()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	RootComponent = PickupMesh;

	PickupMesh->SetSimulatePhysics(true);
	//PickupMesh->SetMassOverrideInKg(EName::NAME_None, 100.0f, true);
	PickupMesh->SetEnableGravity(true);
	PickupMesh->SetConstraintMode(EDOFMode::Default);
	PickupMesh->SetGenerateOverlapEvents(true);
	PickupMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PickupMesh->SetCollisionObjectType(ECC_WorldDynamic);
	PickupMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	PickupMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	PickupMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetSphereRadius(500);
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PickupWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Pickup Text"));
	PickupWidgetComponent->SetWidgetClass(WidgetClass);
	PickupWidgetComponent->SetupAttachment(RootComponent);
}

void APickup::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		SphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnOverlapComponentStart);
		SphereComponent->OnComponentEndOverlap.AddDynamic(this, &APickup::OnOverlapComponentEnd);
	}

	if (PickupWidgetComponent)
	{
		PickupWidget = Cast<UPickupWidget>(PickupWidgetComponent->GetWidget());
		if (PickupWidget)
		{
			PickupWidget->SetPickupText(ItemInstance->Item->GetItemName());
		}
		PickupWidgetComponent->SetVisibility(false);
	}
}



void APickup::OnPickupDataReceived() const
{
	if (ItemInstance->Item->GetItemPickupStaticMesh())
	{
		PickupMesh->SetStaticMesh(ItemInstance->Item->GetItemPickupStaticMesh());
	}
}

/*
void APickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APickup, PickupWidget);
}*/

void APickup::SetPickupData(UItemInstance* InItemInstance, const int32 InQuantity)
{
	if (InItemInstance)
	{
		ItemInstance = InItemInstance;
		Quantity = InQuantity;
	}

	OnPickupDataReceived();
	K2_OnPickupDataReceived();
}

void APickup::SetWidgetVisibility(bool bVisible)
{
	if (PickupWidgetComponent)
	{
		PickupWidgetComponent->SetVisibility(bVisible);
	}
}

void APickup::OnOverlapComponentStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{	
	//SweepResult.Actor
	IInventoryInterface* InventoryInterface = nullptr;
	auto Components = OtherActor->GetComponents();

	for (auto Component : Components)
	{
		InventoryInterface = Cast<IInventoryInterface>(Component);
		if (InventoryInterface)
		{
			break;
		}
	}
	if (InventoryInterface)
	{
		int32 SizeArray = InventoryInterface->GetOverlappingItems().Num();
		UE_LOG(LogTemp, Display, TEXT("%d"), SizeArray);

		//InventoryInterface->LootItem(this, Quantity);
		InventoryInterface->OverlappingItemToArray(this);
		SizeArray = InventoryInterface->GetOverlappingItems().Num();
		UE_LOG(LogTemp, Display, TEXT("%d"), SizeArray);
	}
}

void APickup::OnOverlapComponentEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IInventoryInterface* InventoryInterface = nullptr;
	auto Components = OtherActor->GetComponents();

	for (auto Component : Components)
	{
		InventoryInterface = Cast<IInventoryInterface>(Component);
		if (InventoryInterface)
		{
			break;
		}
	}
	if (InventoryInterface)
	{
		InventoryInterface->RemoveItemFromOverlapping(this);
	}
}