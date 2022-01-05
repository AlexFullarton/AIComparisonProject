// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyControllerBT.h"

AEnemyControllerBT::AEnemyControllerBT()
{
	// Create the structure of the behaviour tree for this instance of the enemy controller
	BehaviourTree::SelectorTreeNode selector_nodes[1];
	BehaviourTree::SequenceTreeNode sequence_nodes[1];

	Action calculateNewPatrolLocation(std::bind(&AEnemyControllerBT::CalculateNewPatrolLocation, this));
}

bool AEnemyControllerBT::CalculateNewPatrolLocation()
{
	return false;
}

bool AEnemyControllerBT::ArrivedAtPatrolLocation()
{
	return false;
}

bool AEnemyControllerBT::MoveToPatrolLocation()
{
	return false;
}

