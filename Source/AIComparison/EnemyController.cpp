// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"

AEnemyController::AEnemyController()
{
	PrimaryActorTick.bCanEverTick = true;
	// Set default destination to be the origin of the world
	destination.Location = FVector(0.0f, 0.0f, 0.0f);
	searchRadius = 1000.0f;
	
	// Enemy can be 25 units away before searching for a new destination
	tolerance = 25.0f;

	hasSpottedPlayer = false;
	patrolSpeed = 100.0f;
	chaseSpeed = 500.0f;

	// How long to wait between each attack (minimum)
	attackRate = 2.0f;
	// Percentage block chance (minimum)
	blockChance = 33.3f;

	// Enemies initially are melee
	isMelee = true;
	isRanged = false;
}

void AEnemyController::MoveToRandomLocationInDistance(FVector pawnLocation)
{
	UNavigationSystemV1* navSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (navSystem)
	{
		// Move the enemy to the given destination if a new location is found
		if (navSystem->GetRandomReachablePointInRadius(pawnLocation, searchRadius, destination))
		{
			MoveToLocation(destination.Location, tolerance);
		}
	}
}

void AEnemyController::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Doesnt seem to be getting the location of the possessed actor
	FVector pawnLocation = GetPawn()->GetActorLocation();
	// If the pawn has reached the given destination
	if ((pawnLocation - destination.Location).IsNearlyZero(tolerance))
	{
		MoveToRandomLocationInDistance(pawnLocation);
	}
}