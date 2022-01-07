// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyControllers/StateMachine/ConcreteEnemyStates.h"

//////////////////
// Patrol state //
//////////////////

// Fires on enemy controller transitioning to this state from another
void EnemyPatrolState::enterState(AEnemyControllerFSM* controller)
{
	// When entering patrol state from another state, set enemy to find 
	// new random location to patrol to
	controller->MoveToRandomLocationInDistance(controller->pawnLocation, controller->patrolSpeed);
}

void EnemyPatrolState::updateState(AEnemyControllerFSM* controller)
{
	// Check death state
	if (controller->isDead)
		controller->setState(EnemyDeathState::getInstance());
	// If enemy is low health then reatreat - only happens once per enemy
	else if (controller->IsCriticalHealth() && !controller->hasRetreated)
		controller->setState(EnemyRetreatState::getInstance());
	// If sensed enemies is anything other than zero, this enemy has
	// detected a player character so must change state
	else if (controller->sensedPlayer != nullptr)
	{
		controller->setState(EnemyChaseState::getInstance());
	}	
	// if no enemies are detected, then the enemy can continue to patrol
	else
	{
		// If the enemy has reached the previously set destination, give it a 
		// new random destination
		if (controller->GetMoveStatus() != EPathFollowingStatus::Moving)
			controller->MoveToRandomLocationInDistance(controller->pawnLocation, controller->patrolSpeed);
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
	// If enemy is low health then reatreat - only happens once per enemy
	else if (controller->IsCriticalHealth() && !controller->hasRetreated)
		controller->setState(EnemyRetreatState::getInstance());
	// If the player is no longer being detected by the enemy
	// change state back to patrol state
	else if (controller->sensedPlayer == nullptr)
	{
		controller->setState(EnemyPatrolState::getInstance());
	}
	// If the enemy is still detecting and chasing the player
	else
	{
		// If the enemy can see 3+ friendlies attacking the player and is in range
		if (controller->sensedFriendlies.Num() >= 3 && controller->getDistanceToPlayer() < controller->rangedAttackRange)
			controller->setState(EnemyRangedAttackState::getInstance());
		// If the enemy has reached the player, change to attack state
		else if (controller->GetMoveStatus() != EPathFollowingStatus::Moving)
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
	// Could be coming from a ranged attack state so check is enemy is melee or ranged
	if (!controller->isMelee)
		controller->SwapWeapons();
	controller->Attack();
	// Set the timer that counts how long until blocking is allowed by this enemy
	controller->SetBlockTimer();
}

void EnemyMeleeAttackState::updateState(AEnemyControllerFSM* controller)
{
	// Check death state
	if (controller->isDead)
		controller->setState(EnemyDeathState::getInstance());
	// If enemy is low health then reatreat - only happens once per enemy
	else if (controller->IsCriticalHealth() && !controller->hasRetreated)
		controller->setState(EnemyRetreatState::getInstance());
	// If the enemy can no longer see the player
	else if (controller->sensedPlayer == nullptr)
		controller->setState(EnemyPatrolState::getInstance());
	// If the enemy can still see the player
	else
	{
		// If blocking has been allowed by the timer
		if (controller->IsBlockAllowed())
			controller->setState(EnemyDefendState::getInstance());
		// If the player is no longer in range for a melee attack
		else if (controller->getDistanceToPlayer() > controller->meleeAttackRange)
			controller->setState(EnemyChaseState::getInstance());
		// If the player is in range then attack
		else
			controller->Attack();
	}
}

void EnemyMeleeAttackState::exitState(AEnemyControllerFSM* controller)
{
	controller->disallowAttack();
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
	// Could be coming from a melee attack state so check is enemy is melee or ranged
	if (!controller->isRanged)
		controller->SwapWeapons();
	controller->AttackRanged();
}

void EnemyRangedAttackState::updateState(AEnemyControllerFSM* controller)
{
	// Check death state
	if (controller->isDead)
		controller->setState(EnemyDeathState::getInstance());
	// If enemy is low health then reatreat - only happens once per enemy
	else if (controller->IsCriticalHealth() && !controller->hasRetreated)
		controller->setState(EnemyRetreatState::getInstance());
	// If the enemy can no longer see the player
	else if (controller->sensedPlayer == nullptr)
		controller->setState(EnemyPatrolState::getInstance());
	// If the enemy can still see the player
	else
	{
		// If the player has moved closed to this enemy
		if (controller->getDistanceToPlayer() < controller->meleeAttackRange)
			controller->setState(EnemyMeleeAttackState::getInstance());
		// If the player is no longer in range for a ranged attack
		if (controller->getDistanceToPlayer() > controller->rangedAttackRange)
			controller->setState(EnemyChaseState::getInstance());
		// If the player is in range and not already attacking;
		else if (!controller->IsEnemyAttacking() && controller->IsRangedAllowed())
			controller->AttackRanged();
	}
}

EnemyState& EnemyRangedAttackState::getInstance()
{
	static EnemyRangedAttackState singleton;
	return singleton;
}

//////////////////
// Defend state //
//////////////////

void EnemyDefendState::enterState(AEnemyControllerFSM* controller)
{
	// Entered from other state - immediately start blocking
	controller->Block();
	controller->SetAttackTimer();
}

void EnemyDefendState::updateState(AEnemyControllerFSM* controller)
{
	// Check death state
	if (controller->isDead)
		controller->setState(EnemyDeathState::getInstance());
	// If enemy is low health then reatreat - only happens once per enemy
	else if (controller->IsCriticalHealth() && !controller->hasRetreated)
		controller->setState(EnemyRetreatState::getInstance());
	// If the enemy can no longer see the player
	else if (controller->sensedPlayer == nullptr)
		controller->setState(EnemyPatrolState::getInstance());
	// If the enemy can still see the player
	else
	{
		// If blocking has been allowed by the timer
		if (controller->IsAttackAllowed())
			controller->setState(EnemyMeleeAttackState::getInstance());
		// If the player is no longer in range for a melee attack
		else if (controller->getDistanceToPlayer() > controller->meleeAttackRange)
			controller->setState(EnemyChaseState::getInstance());
	}
}

void EnemyDefendState::exitState(AEnemyControllerFSM* controller)
{
	controller->disallowBlock();
	controller->StopBlocking();
}

EnemyState& EnemyDefendState::getInstance()
{
	static EnemyDefendState singleton;
	return singleton;
}

///////////////////
// Retreat state //
///////////////////

void EnemyRetreatState::enterState(AEnemyControllerFSM* controller)
{
	controller->MoveToRandomLocationInDistance(controller->pawnLocation, controller->chaseSpeed);
	controller->hasRetreated = true;
}

void EnemyRetreatState::updateState(AEnemyControllerFSM* controller)
{
	// Check death state
	if (controller->isDead)
		controller->setState(EnemyDeathState::getInstance());
	// If point has been reached
	else if (controller->GetMoveStatus() != EPathFollowingStatus::Moving)
		controller->setState(EnemyPatrolState::getInstance());
}

EnemyState& EnemyRetreatState::getInstance()
{
	static EnemyRetreatState singleton;
	return singleton;
}

/////////////////
// Death state //
/////////////////

void EnemyDeathState::enterState(AEnemyControllerFSM* controller)
{
	controller->EnemyKilled();
}

void EnemyDeathState::updateState(AEnemyControllerFSM* controller)
{
	// Once the enemy is in the death state, they can no longer change to any other states
}

EnemyState& EnemyDeathState::getInstance()
{
	static EnemyDeathState singleton;
	return singleton;
}