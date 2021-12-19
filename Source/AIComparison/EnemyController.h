// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "PlayerCharacter.h"
#include "GenericTeamAgentInterface.h"
#include "EnemyController.generated.h"

class AEnemyCharacter;

/**
 * 
 */
UCLASS()
class AICOMPARISON_API AEnemyController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyController();

	void OnPossess(APawn* InPawn);

	virtual void Tick(float DeltaTime);

	void MoveToRandomLocationInDistance(FVector pawnLocation);

	void MoveToPlayer(float acceptanceRadius);

	float getDistanceToPlayer();

	void Attack();

	void AttackRanged();

	bool IsEnemyAttacking();

	void SwapWeapons();

	UFUNCTION()
	void PerceptionUpdated(const TArray<AActor*>& testActors);

	ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

protected:
	virtual void BeginPlay();

public:
	// AI Perception component - for enemy sight
	UAIPerceptionComponent* PerceptionComponent;
	UAISenseConfig_Sight* sightConfig;

	// Controlled Enemy
	AEnemyCharacter* controlledEnemy;
	// Controlled Enemy's location
	FVector pawnLocation;
	// Destination that the enemy is trying to path to
	FNavLocation destination;
	// Search distance for a new point to path to (radius)
	float searchRadius;
	// Destination tolerance - how close does the enemy need to get before searching for a new destination
	float tolerance;

	// Arrays of sensed actors
	TArray<AActor*> sensedFriendlies;
	AActor* sensedPlayer;

	// Movement speed variables
	float patrolSpeed;
	float chaseSpeed;

	// Combat variables
	float meleeAttackRange;
	float rangedAttackRange;
	float meleeTolerance;
	float rangedTolerance;
	float attackRate;
	float blockChance;
	bool isMelee;
	bool isRanged;
	bool isDead;
};
