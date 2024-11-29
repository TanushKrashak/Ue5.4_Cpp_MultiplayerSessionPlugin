// Copyright Epic Games, Inc. All Rights Reserved.

#include "Cpp_SessionPluginGameMode.h"
#include "Cpp_SessionPluginCharacter.h"
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
