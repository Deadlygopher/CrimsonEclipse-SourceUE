// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameMode.h"

#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CRIMSONECLIPSE_API ALobbyGameMode : public AGameMode //Base
{
	GENERATED_BODY()

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	
};
