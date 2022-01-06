// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyControllers/EnemyController.h"
#include "EnemyControllers/BehaviourTree/BehaviourTree.h"
#include <memory>
#include "EnemyControllerBT.generated.h"

UCLASS()
class AICOMPARISON_API AEnemyControllerBT : public AEnemyController
{
	GENERATED_BODY()

public:
	AEnemyControllerBT();

	virtual void OnPossess(APawn* InPawn);
	virtual void Tick(float DeltaTime);

private:
	// Custom classes that hold the structure of the behaviour tree - tree will be built at runtime
	BehaviourTree behaviourTree;

	BehaviourTree::SelectorTreeNode selector_nodes[1];
	BehaviourTree::SequenceTreeNode sequence_nodes[1];
	BehaviourTree::RepeaterTreeNode repeater_node;

	Action actions[3] = { Action(std::bind(&AEnemyControllerBT::ArrivedAtPatrolLocation, this)), Action(std::bind(&AEnemyControllerBT::CalculateNewPatrolLocation, this)), Action(std::bind(&AEnemyControllerBT::MoveToPatrolLocation, this)) };

	// Action functions - passed as pointers to the action nodes/leaves of the tree
	NodeStatus EnemyDeath();
	NodeStatus CalculateNewPatrolLocation();
	NodeStatus ArrivedAtPatrolLocation();
	NodeStatus MoveToPatrolLocation();
};