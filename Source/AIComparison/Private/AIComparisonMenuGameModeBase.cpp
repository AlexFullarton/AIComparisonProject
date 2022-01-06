// Fill out your copyright notice in the Description page of Project Settings.


#include "AIComparisonMenuGameModeBase.h"
#include "Characters/Player/PlayerCharacter.h"
#include "PlayerControllers/PlayerMenuController.h"

AAIComparisonMenuGameModeBase::AAIComparisonMenuGameModeBase()
	: Super()
{
	// Set default pawn class to our custom blueprint character base
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("Class'/Script/AIComparison.PlayerCharacter'"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// Use custom player controller menu class
	PlayerControllerClass = APlayerMenuController::StaticClass();
}