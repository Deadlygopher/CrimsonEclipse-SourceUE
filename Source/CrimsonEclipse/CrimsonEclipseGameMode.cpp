// Copyright Epic Games, Inc. All Rights Reserved.

#include "CrimsonEclipseGameMode.h"
#include "CrimsonEclipsePlayerController.h"
#include "CrimsonEclipseCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACrimsonEclipseGameMode::ACrimsonEclipseGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ACrimsonEclipsePlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}