// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if (GameState)
	{
		int32 NumberOfPlayers = GameState->PlayerArray.Num();

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1, 60, FColor::Blue,
											FString::Printf(TEXT("Players Num : %d"), NumberOfPlayers));

			APlayerState* PlayerState = NewPlayer->GetPlayerState<APlayerState>();
			if (PlayerState)
			{
				FString PlayerName = PlayerState->GetPlayerName();

				GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Cyan,
												FString::Printf(TEXT("%s has joined the game!"), *PlayerName));
			}
		}
		if (NumberOfPlayers >= 4)
		{
			UWorld* World = GetWorld();
			if (World)
			{
				bUseSeamlessTravel = true;
				World->ServerTravel(FString("/Game/TopDownCPP/Maps/PlayMap?listen"));
			}
		}
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	if (GEngine) 
	{
		int32 NumberOfPlayers = GameState->PlayerArray.Num();

		GEngine->AddOnScreenDebugMessage(1, 60, FColor::Blue,
										FString::Printf(TEXT("Players Num : %d"), NumberOfPlayers - 1));

		APlayerState* PlayerState = Exiting->GetPlayerState<APlayerState>();
		if (PlayerState)
		{
			FString PlayerName = PlayerState->GetPlayerName();

			GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Cyan,
											FString::Printf(TEXT("%s has left the game!"), *PlayerName));
		}
	}
}
