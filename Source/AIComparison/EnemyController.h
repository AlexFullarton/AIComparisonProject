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
#include "Kismet/KismetMathLibrary.h"
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
	virtual void OnPossess(APawn* InPawn);
	virtual void Tick(float DeltaTime);

	void MoveToRandomLocationInDistance(FVector pawnLocation, float speed);
	void MoveToPlayer(float acceptanceRadius);
	float getDistanceToPlayer();
	void RotateToFacePlayer();

	void Attack();
	void AttackRanged();
	bool IsEnemyAttacking();
	void SetAttackTimer();
	void allowAttack();
	void disallowAttack();
	bool IsAttackAllowed() { return attackAllowed; }
	void SetRangedAttackTimer();
	void allowRanged();
	void disallowRanged();
	bool IsRangedAllowed() { return rangedAllowed; }

	void Block();
	void StopBlocking();
	void SetBlockTimer();
	void allowBlock();
	void disallowBlock();
	bool IsBlockAllowed() { return blockAllowed; }

	void SwapWeapons();

	bool IsCriticalHealth();

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

	bool attackAllowed;
	bool rangedAllowed;
	bool blockAllowed;
	bool isMelee;
	bool isRanged;
	bool hasRetreated;
	bool isDead;

	// TimerHandle for block/attack timers
	FTimerHandle TimerHandle;
};
