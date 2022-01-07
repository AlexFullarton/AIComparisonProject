// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyControllers/EnemyController.h"
#include <functional>
#include <memory>
#include "SelectorNode.h"
#include "SequenceNode.h"
#include "InverterNode.h"
#include "SucceederNode.h"
#include "FailerNode.h"
#include "RepeaterNode.h"
#include "RepeatUntilFailNode.h"
#include "Action.h"
#include "EnemyControllerBT.generated.h"

UCLASS()
class AICOMPARISON_API AEnemyControllerBT : public AEnemyController
{
	GENERATED_BODY()

public:
	AEnemyControllerBT();

	virtual void OnPossess(APawn* InPawn);
	virtual void Tick(float DeltaTime);

	// Create the structure of the behaviour tree
	void BuildTree();
	// Evaulate the tree at run time
	NodeStatus RunTree();

private:
	// Tree nodes that will be used to build the tree
	SelectorNode RootNode;
	SequenceNode PatrolNode;

	// Action nodes - these nodes are what add functionality to the tree

	// To check death state
	Action CheckIfDead = Action(std::bind(&AEnemyControllerBT::EnemyDeath, this));

	// To allow enemy to patrol to random location in set distance
	Action CalculatePatrolDestination = Action(std::bind(&AEnemyControllerBT::CalculateNewPatrolLocation, this));
	Action MoveToPatrolDestination = Action(std::bind(&AEnemyControllerBT::MoveToPatrolLocation, this));
	Action WaitForMove = Action(std::bind(&AEnemyControllerBT::CheckAtPatrolLocation, this));

	// Action functions - passed as pointers to the action nodes/leaves of the tree
	NodeStatus EnemyDeath();

	NodeStatus CalculateNewPatrolLocation();
	NodeStatus MoveToPatrolLocation();
	NodeStatus CheckAtPatrolLocation();
};