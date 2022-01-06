// Copyright Epic Games, Inc. All Rights Reserved.


#include "AIComparisonGameModeBase.h"
#include "Characters/Player/PlayerHUD.h"
#include "Characters/Player/PlayerCharacter.h"
#include "PlayerControllers/PlayerCharacterController.h"

AAIComparisonGameModeBase::AAIComparisonGameModeBase()
	: Super()
{
	// Set default pawn class to our custom blueprint character base
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("Class'/Script/AIComparison.PlayerCharacter'"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// Use our custom hud class
	HUDClass = APlayerHUD::StaticClass();

	// Use custom player controller class
	PlayerControllerClass = APlayerCharacterController::StaticClass();
}