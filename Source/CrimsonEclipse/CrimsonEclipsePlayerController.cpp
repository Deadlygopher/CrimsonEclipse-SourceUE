// Copyright Epic Games, Inc. All Rights Reserved.

#include "CrimsonEclipsePlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "CrimsonEclipseCharacter.h"
#include "Engine/World.h"
#include "Pickup.h"
#include "InventoryInterface.h"

ACrimsonEclipsePlayerController::ACrimsonEclipsePlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void ACrimsonEclipsePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}
}

void ACrimsonEclipsePlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &ACrimsonEclipsePlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &ACrimsonEclipsePlayerController::OnSetDestinationReleased);

	// set key for pickup items from inventory component
	InputComponent->BindAction("PickupItem", IE_Pressed, this, &ACrimsonEclipsePlayerController::PickupItem);

	// set key for show/hide pickup widget
	InputComponent->BindAction("ShowPickupWidget", IE_Pressed, this, &ACrimsonEclipsePlayerController::ShowPickupWidget);

	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ACrimsonEclipsePlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ACrimsonEclipsePlayerController::MoveToTouchLocation);
}

void ACrimsonEclipsePlayerController::MoveToMouseCursor()
{
	// Trace to see what is under the mouse cursor
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		// We hit something, move there
		RequestSetNewMoveDestination(Hit.ImpactPoint);
		//SetNewMoveDestination(Hit.ImpactPoint);
	}
}

void ACrimsonEclipsePlayerController::PickupItem()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> QueryArray;
	QueryArray.Add(UEngineTypes::ConvertToObjectType(ECC_Visibility));
	FHitResult Hit;

	GetHitResultUnderCursorForObjects(QueryArray, true, Hit);
	APickup* ActorToPickup = Cast<APickup>(Hit.GetActor());
	if (ActorToPickup)
	{
		ACrimsonEclipseCharacter* ControlledCharacter = Cast<ACrimsonEclipseCharacter>(GetPawn());
		if (ControlledCharacter)
		{
			IInventoryInterface* InventoryInterface = nullptr;
			auto Components = ControlledCharacter->GetComponents();
			for (auto Component : Components)
			{
				InventoryInterface = Cast<IInventoryInterface>(Component);
				if (InventoryInterface != nullptr)
				{
					auto ItemsToLoot = InventoryInterface->GetOverlappingItems();
					if (ItemsToLoot.Contains(ActorToPickup))
					{
						if (InventoryInterface->LootItem(ActorToPickup, ActorToPickup->Quantity))
						{
							break;
						}
						else
						{
							continue;
						}
					}
				}
			}
		}
	}
}

void ACrimsonEclipsePlayerController::ShowPickupWidget()
{
	bShowWidget = !bShowWidget;
	ACrimsonEclipseCharacter* ControlledCharacter = Cast<ACrimsonEclipseCharacter>(GetPawn());
	IInventoryInterface* InventoryInterface = nullptr;
	auto Components = ControlledCharacter->GetComponents();
	for (auto Component : Components)
	{
		InventoryInterface = Cast<IInventoryInterface>(Component);
		if (InventoryInterface != nullptr)
		{
			InventoryInterface->SwitchItemWidgetVisibility(bShowWidget);
		}
	}
}



void ACrimsonEclipsePlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(HitResult.ImpactPoint);
	}
}


// Requests a destination set for the client and server.
void ACrimsonEclipsePlayerController::RequestSetNewMoveDestination(const FVector DestLocation)
{
	ClientSetNewMoveDestination(DestLocation);
	ServerSetNewMoveDestination(DestLocation);
}



// Requests a destination set for the client (Comes first, since client calls it by clicking).
void ACrimsonEclipsePlayerController::ClientSetNewMoveDestination_Implementation(const FVector DestLocation)
{
	SetNewMoveDestination(DestLocation);
}

// Requests a destination set for the server (Comes after, to replicate client movement server-side).
void ACrimsonEclipsePlayerController::ServerSetNewMoveDestination_Implementation(const FVector DestLocation)
{
	SetNewMoveDestination(DestLocation);
}


// Common destination setting and movement implementation.
void ACrimsonEclipsePlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if ((Distance > 120.0f))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void ACrimsonEclipsePlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
}

void ACrimsonEclipsePlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}

