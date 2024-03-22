// Fill out your copyright notice in the Description page of Project Settings.


#include "OverheadWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "CrimsonEclipse/CrimsonEclipseComponents/HealthComponent.h"

void UOverheadWidget::UpdateHealthBar(float Health, float MaxHealth) const
{
	if (DisplayHealth && HealthBar && MaxHealth > 0.f)
	{
		DisplayHealth->SetText(FText::FromString(FString::FromInt(Health)));
		HealthBar->SetPercent(Health / MaxHealth);
	}
}


void UOverheadWidget::NativeDestruct()
{
	//RemoveFromParent();
	Super::NativeDestruct();
}
