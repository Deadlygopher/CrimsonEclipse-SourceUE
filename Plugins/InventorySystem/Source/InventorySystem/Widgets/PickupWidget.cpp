// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupWidget.h"
#include "Components/TextBlock.h"

bool UPickupWidget::Initialize()
{
	return Super::Initialize();

	//PickupText = CreateDefaultSubobject<UTextBlock>("Pickup Text");
}

void UPickupWidget::SetPickupText(FText TextToSet)
{
	if (PickupText)
	{
		PickupText->SetText(TextToSet);
	}
}
