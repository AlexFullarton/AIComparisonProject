// Fill out your copyright notice in the Description page of Project Settings.


#include "ConcreteEnemyStates.h"

// Patrol state
void EnemyPatrolState::updateState(AEnemyControllerFSM* enemy)
{
	// This is where we determine which state to change to based on data from enemy
	// e.g. if low health then defence, if player seen then chase, if really low health then run
	enemy->setState(EnemyPatrolState::getInstance());
}

EnemyState& EnemyPatrolState::getInstance()
{
	static EnemyPatrolState singleton;
	return singleton;
}

// Chase state
void EnemyChaseState::updateState(AEnemyControllerFSM* enemy)
{
	// This is where we determine which state to change to
	enemy->setState(EnemyChaseState::getInstance());
}

EnemyState& EnemyChaseState::getInstance()
{
	static EnemyChaseState singleton;
	return singleton;
}

// Attack state
void EnemyAttackState::updateState(AEnemyControllerFSM* enemy)
{
	// This is where we determine which state to change to
	enemy->setState(EnemyAttackState::getInstance());
}

EnemyState& EnemyAttackState::getInstance()
{
	static EnemyAttackState singleton;
	return singleton;
}

// Retreat state
void EnemyRetreatState::updateState(AEnemyControllerFSM* enemy)
{
	// This is where we determine which state to change to
	enemy->setState(EnemyRetreatState::getInstance());
}

EnemyState& EnemyRetreatState::getInstance()
{
	static EnemyRetreatState singleton;
	return singleton;
}

// Defend state
void EnemyDefendState::updateState(AEnemyControllerFSM* enemy)
{
	// This is where we determine which state to change to
	enemy->setState(EnemyDefendState::getInstance());
}

EnemyState& EnemyDefendState::getInstance()
{
	static EnemyDefendState singleton;
	return singleton;
}