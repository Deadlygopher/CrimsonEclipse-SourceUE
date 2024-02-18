// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickup.h"
#include "InventoryInterface.h"
#include "ItemInstance.h"
#include "Item.h"
#include "Components/SphereComponent.h"


APickup::APickup()
{
	PrimaryActorTick.bCanEverTick = true;

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

	SphereComponent = CreateDefaultSubobject<USphereComponent>("Collision");
	SphereComponent->SetupAttachment(RootComponent);
	//SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	//SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnOverlapComponent);
}

void APickup::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		//SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		//SphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnOverlapComponent);
	}
}

void APickup::OnPickupDataReceived() const
{
	if (ItemInstance->Item->GetItemPickupStaticMesh())
	{
		PickupMesh->SetStaticMesh(ItemInstance->Item->GetItemPickupStaticMesh());
	}
}

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

void APickup::OnOverlapComponent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1, 15.f, FColor::Green,
			FString(TEXT("Loooooot")));
	}

	
	IInventoryInterface* InventoryInterface = nullptr;
	int32& QuantityRef = Quantity;
	auto Components = OtherActor->GetComponents();
	for (auto Component : Components)
	{
		InventoryInterface = Cast<IInventoryInterface>(Component);
		if (InventoryInterface)
		{
			break;
		}
	}

	//IInventoryInterface* InventoryInterface = Cast<IInventoryInterface>(OtherActor);
	if (InventoryInterface)
	{
		InventoryInterface->LootItem(this, Quantity);
	}
}
