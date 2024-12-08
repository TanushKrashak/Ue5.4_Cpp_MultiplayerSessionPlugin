// Copyright Epic Games, Inc. All Rights Reserved.

#include "Cpp_SessionPluginGameMode.h"
#include "Cpp_SessionPluginCharacter.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "UObject/ConstructorHelpers.h"

ACpp_SessionPluginGameMode::ACpp_SessionPluginGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ACpp_SessionPluginGameMode::PostLogin(APlayerController* NewPlayer) {
	Super::PostLogin(NewPlayer);

	if (GameState) {
		int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Yellow, FString::Printf(TEXT("Number of Players: %d"), NumberOfPlayers));

		if (APlayerState* PlayerState = NewPlayer->GetPlayerState<APlayerState>()) {
			// print player name
			GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Yellow, FString::Printf(TEXT("Player Name: %s Has Joined"), *PlayerState->GetPlayerName()));
		}
	}
}
void ACpp_SessionPluginGameMode::Logout(AController* Exiting) {
	Super::Logout(Exiting);

	if (GameState) {
		int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Yellow, FString::Printf(TEXT("Number of Players: %d"), NumberOfPlayers - 1));

		if (APlayerState* PlayerState = Exiting->GetPlayerState<APlayerState>()) {
			// print player name
			GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Yellow, FString::Printf(TEXT("Player Name: %s Has Left"), *PlayerState->GetPlayerName()));
		}
	}
}
