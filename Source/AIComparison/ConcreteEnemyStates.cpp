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
	// Maybe pass a variable here depending on ranged or melee, to allow larger distances between ranged enemies and the player
	controller->MoveToPlayer();
}

void EnemyChaseState::updateState(AEnemyControllerFSM* controller)
{
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
			controller->setState(EnemyAttackState::getInstance());
	}
}

EnemyState& EnemyChaseState::getInstance()
{
	static EnemyChaseState singleton;
	return singleton;
}

//////////////////
// Attack state //
//////////////////

// Fires on enemy controller transitioning to this state from another
void EnemyAttackState::enterState(AEnemyControllerFSM* controller)
{
	// When enterine from any other state, first action should be to try
	//  and attack the player
	controller->Attack();
}

void EnemyAttackState::updateState(AEnemyControllerFSM* controller)
{
	// This is where we determine which state to change to
	controller->setState(EnemyAttackState::getInstance());
}

EnemyState& EnemyAttackState::getInstance()
{
	static EnemyAttackState singleton;
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
	// This is where we determine which state to change to
	controller->setState(EnemyDefendState::getInstance());
}

EnemyState& EnemyDefendState::getInstance()
{
	static EnemyDefendState singleton;
	return singleton;
}