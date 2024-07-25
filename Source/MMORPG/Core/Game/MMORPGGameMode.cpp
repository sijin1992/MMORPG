// Copyright Epic Games, Inc. All Rights Reserved.

#include "MMORPGGameMode.h"
#include "MMORPGCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "MMORPGHUD.h"

AMMORPGGameMode::AMMORPGGameMode()
{
	HUDClass = AMMORPGHUD::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
