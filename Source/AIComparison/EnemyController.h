// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class AICOMPARISON_API AEnemyController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyController();

	void MoveToRandomLocationInDistance(FVector pawnLocation);

	virtual void Tick(float DeltaTime);

protected:
	virtual void BeginPlay();

public:
	// Destination that the enemy is trying to path to
	FNavLocation destination;
	// Search distance for a new point to path to (radius)
	float searchRadius;
	// Destination tolerance - how close does the enemy need to get before searching for a new destination
	float tolerance;

	// Has the enemy spotted the player
	bool hasSpottedPlayer;

	// Movement speed variables
	float patrolSpeed;
	float chaseSpeed;

	// Combat variables
	float attackRate;
	float blockChance;
	bool isMelee;
	bool isRanged;
	
};
