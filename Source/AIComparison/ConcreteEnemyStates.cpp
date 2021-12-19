// Fill out your copyright notice in the Description page of Project Settings.


#include "ConcreteEnemyStates.h"

//////////////////
// Patrol state //
//////////////////

// Fires on enemy controller transitioning to this state from another
void EnemyPatrolState::enterState(AEnemyControllerFSM* controller)
{
	// When entering patrol state from another state, set enemy to find 
	// new random location to patrol to
	controller->MoveToRandomLocationInDistance(controller->pawnLocation);
}

void EnemyPatrolState::updateState(AEnemyControllerFSM* controller)
{
	// Check death state
	if (controller->isDead)
		controller->setState(EnemyDeathState::getInstance());
	// This is where we determine which state to change to based on data from enemy
	// e.g. if low health then defence, if player seen then chase, if really low health then run

	// If sensed enemies is anything other than zero, this enemy has
	// detected a player character so must change state
	if (controller->sensedPlayer != nullptr)
	{
		controller->setState(EnemyChaseState::getInstance());
	}	
	// if no enemies are detected, then the enemy can continue to patrol
	else
	{
		// If the enemy has reached the previously set destination, give it a 
		// new random destination
		if (controller->GetMoveStatus() != EPathFollowingStatus::Moving)
			controller->MoveToRandomLocationInDistance(controller->pawnLocation);
	}
}

void EnemyPatrolState::exitState(AEnemyControllerFSM* controller)
{
	// On leaving the patrol state when the player has been spotted,
	// set this enemy to be either ranged or melee based on some ratio
	//int attackType = FMath::RandRange(0, 4);
	//if (attackType == 0)
	{
		// Enemies start out as melee so swapping will equip the bow
		controller->SwapWeapons();
	}
}

EnemyState& EnemyPatrolState::getInstance()
{
	static EnemyPatrolState singleton;
	return singleton;
}

/////////////////
// Chase state //
/////////////////

// Fires on enemy controller transitioning to this state from another
void EnemyChaseState::enterState(AEnemyControllerFSM* controller)
{
	// When entering this state from any other, enemy needs to be set to chasing the player
	// Value for acceptanceRadius passed to the controller is dependant on the enemy 
	// being ranged or melee
	if (controller->isMelee)
		controller->MoveToPlayer(controller->meleeTolerance);
	else
		controller->MoveToPlayer(controller->rangedTolerance);
}

void EnemyChaseState::updateState(AEnemyControllerFSM* controller)
{
	// Check death state
	if (controller->isDead)
		controller->setState(EnemyDeathState::getInstance());
	// If the player is no longer being detected by the enemy
	// change state back to patrol state
	if (controller->sensedPlayer == nullptr)
	{
		controller->setState(EnemyPatrolState::getInstance());
	}
	// If the enemy is still detecting and chasing the player
	else
	{
		// If the enemy has reached the player, change to attack state
		if (controller->GetMoveStatus() != EPathFollowingStatus::Moving)
			if (controller->isMelee)
				controller->setState(EnemyMeleeAttackState::getInstance());
			else
				controller->setState(EnemyRangedAttackState::getInstance());
	}
}

EnemyState& EnemyChaseState::getInstance()
{
	static EnemyChaseState singleton;
	return singleton;
}

////////////////////////
// Melee Attack state //
////////////////////////

// Fires on enemy controller transitioning to this state from another
void EnemyMeleeAttackState::enterState(AEnemyControllerFSM* controller)
{
	// When enterine from any other state, first action should be to try
	// and attack the player as the enemy should already be within
	// attacking distance
	controller->Attack();
}

void EnemyMeleeAttackState::updateState(AEnemyControllerFSM* controller)
{
	// Check death state
	if (controller->isDead)
		controller->setState(EnemyDeathState::getInstance());
	// If the enemy can no longer see the player
	if (controller->sensedPlayer == nullptr)
		controller->setState(EnemyPatrolState::getInstance());
	// If the enemy can still see the player
	else
	{
		// If the player is no longer in range for a melee attack
		if (controller->getDistanceToPlayer() > controller->meleeAttackRange)
			controller->MoveToPlayer(controller->meleeTolerance);
		// If the player is in range then attack
		else
			controller->Attack();
	}
}

EnemyState& EnemyMeleeAttackState::getInstance()
{
	static EnemyMeleeAttackState singleton;
	return singleton;
}

/////////////////////////
// Ranged Attack state //
/////////////////////////

// Fires on enemy controller transitioning to this state from another
void EnemyRangedAttackState::enterState(AEnemyControllerFSM* controller)
{
	// When enterine from any other state, first action should be to try
	// and attack the player as the enemy should already be within
	// attacking distance
	controller->AttackRanged();
}

void EnemyRangedAttackState::updateState(AEnemyControllerFSM* controller)
{
	// Check death state
	if (controller->isDead)
		controller->setState(EnemyDeathState::getInstance());
	// If the enemy can no longer see the player
	if (controller->sensedPlayer == nullptr)
		controller->setState(EnemyPatrolState::getInstance());
	// If the enemy can still see the player
	else
	{
		// If the player is no longer in range for a ranged attack
		if (controller->getDistanceToPlayer() > controller->rangedAttackRange)
			controller->MoveToPlayer(controller->rangedTolerance);
		// If the player is in range then attack
		else
			controller->AttackRanged();
	}
}

EnemyState& EnemyRangedAttackState::getInstance()
{
	static EnemyRangedAttackState singleton;
	return singleton;
}

// Retreat state
void EnemyRetreatState::updateState(AEnemyControllerFSM* controller)
{
	// This is where we determine which state to change to
	controller->setState(EnemyRetreatState::getInstance());
}

EnemyState& EnemyRetreatState::getInstance()
{
	static EnemyRetreatState singleton;
	return singleton;
}

// Defend state
void EnemyDefendState::updateState(AEnemyControllerFSM* controller)
{
	// Check death state
	if (controller->isDead)
		controller->setState(EnemyDeathState::getInstance());
	// This is where we determine which state to change to
	controller->setState(EnemyDefendState::getInstance());
}

EnemyState& EnemyDefendState::getInstance()
{
	static EnemyDefendState singleton;
	return singleton;
}

// Death state
void EnemyDeathState::updateState(AEnemyControllerFSM* controller)
{
	// Once the enemy is in the death state, they can no longer change to any other states
}

EnemyState& EnemyDeathState::getInstance()
{
	static EnemyDeathState singleton;
	return singleton;
}