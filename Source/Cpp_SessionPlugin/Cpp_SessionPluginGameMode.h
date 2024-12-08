// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Cpp_SessionPluginGameMode.generated.h"

UCLASS(minimalapi)
class ACpp_SessionPluginGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	ACpp_SessionPluginGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
};



