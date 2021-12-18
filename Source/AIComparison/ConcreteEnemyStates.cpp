// Fill out your copyright notice in the Description page of Project Settings.


#include "ConcreteEnemyStates.h"

// Patrol state
void EnemyPatrolState::toggleState(AEnemyController* enemy)
{
	// This is where we determine which state to change to
	enemy->setState(EnemyPatrolState::getInstance());
}

EnemyState& EnemyPatrolState::getInstance()
{
	static EnemyPatrolState singleton;
	return singleton;
}

// Chase state
void EnemyChaseState::toggleState(AEnemyController* enemy)
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
void EnemyAttackState::toggleState(AEnemyController* enemy)
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
void EnemyRetreatState::toggleState(AEnemyController* enemy)
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
void EnemyDefendState::toggleState(AEnemyController* enemy)
{
	// This is where we determine which state to change to
	enemy->setState(EnemyDefendState::getInstance());
}

EnemyState& EnemyDefendState::getInstance()
{
	static EnemyDefendState singleton;
	return singleton;
}