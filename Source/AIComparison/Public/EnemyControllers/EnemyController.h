// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Characters/Player/PlayerCharacter.h"
#include "GenericTeamAgentInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnemyController.generated.h"

class AEnemyCharacter;

UCLASS()
class AICOMPARISON_API AEnemyController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyController();
	virtual void OnPossess(APawn* InPawn);
	virtual void Tick(float DeltaTime);

	// Move enemy to destination
	void MoveToRandomLocationInDistance(FVector pawnLocation, float speed);
	// Move enemy to player location
	void MoveToPlayer(float acceptanceRadius);
	// Calculate distance between the controlled enemy and the player
	float getDistanceToPlayer();
	// Rotate the controlled enemy to face the players location
	void RotateToFacePlayer();

	// Initiate melee attack
	void Attack();
	// Initiate ranged attack
	void AttackRanged();
	// Checks if the controlled enemy is already attacking - used for playing anim montages
	bool IsEnemyAttacking();
	// Set a random amount of time for the current attack period to last for - based on enemy attack rate float
	void SetAttackTimer();
	// Set attack gate bool - prevents attacking again while already attacking/spam clicking for more ranged attacks etc.
	void allowAttack();
	// Unset attack gate bool
	void disallowAttack();
	// Get attack gate bool
	bool IsAttackAllowed() { return attackAllowed; }
	// Set a random amount of time for the current attack period to last for - based on enemy attack rate float
	void SetRangedAttackTimer();
	// Set attack gate bool - prevents attacking again while already attacking/spam clicking for more ranged attacks etc.
	void allowRanged();
	// Unset attack gate bool
	void disallowRanged();
	// Get ranged attack gate bool
	bool IsRangedAllowed() { return rangedAllowed; }

	// Initiate blocking
	void Block();
	// Cancel blocking
	void StopBlocking();
	// Set a random amount of time for the current block to last for (between 1.5 and 3 seconds)
	void SetBlockTimer();
	// Set block gate bool - prevents being abld to block while already blocking/spam clicking block
	void allowBlock();
	// Unset block gate bool
	void disallowBlock();
	// Get block gate bool
	bool IsBlockAllowed() { return blockAllowed; }

	// Switch attack states between melee and ranged
	void SwapWeapons();

	// Enemy death functions

	// Checks enemy health percentage - flee at low hp
	bool IsCriticalHealth();
	// Check if enemy health has been reduced to zero
	void EnemyKilled();

	// Enemy perception functions

	// Fires when the controlled enemy detects/undetects another pawn
	UFUNCTION()
	void PerceptionUpdated(const TArray<AActor*>& testActors);

	// Checks the controlled enemies team against the detected pawn
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

	bool attackAllowed;
	bool rangedAllowed;
	bool blockAllowed;
	bool isMelee;
	bool isRanged;
	bool hasRetreated;
	bool isDead;
	bool isRetreating;

	// TimerHandle for block/attack timers
	FTimerHandle TimerHandle;
};
