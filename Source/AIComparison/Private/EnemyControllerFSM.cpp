// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyControllerFSM.h"
#include "ConcreteEnemyStates.h"

AEnemyControllerFSM::AEnemyControllerFSM()
{
	// Set enemy initial state to patrol
	currentState = &EnemyPatrolState::getInstance();
}

void AEnemyControllerFSM::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Each time the controller is updated, update the state for the controlled enemy
	updateState();
}

void AEnemyControllerFSM::setState(EnemyState& newState)
{
	// Run some code before we exit the current state
	currentState->exitState(this);
	// Change state to the new state
	currentState = &newState;
	// Run some code on entering the new state
	currentState->enterState(this);
}

void AEnemyControllerFSM::updateState()
{
	// Current state determines what the next state will be
	currentState->updateState(this);
}