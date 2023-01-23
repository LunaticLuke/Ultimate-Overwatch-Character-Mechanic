// Copyright Epic Games, Inc. All Rights Reserved.

#include "CMP302CourseworkGameMode.h"
#include "CMP302CourseworkHUD.h"
#include "CMP302CourseworkCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACMP302CourseworkGameMode::ACMP302CourseworkGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ACMP302CourseworkHUD::StaticClass();
}
