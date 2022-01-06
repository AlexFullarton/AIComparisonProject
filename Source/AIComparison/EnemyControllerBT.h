// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyController.h"
#include "BehaviourTree.h"
#include <memory>
#include "EnemyControllerBT.generated.h"

/**
 * 
 */
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

	// Action functions - passed as pointers to the action nodes/leaves of the tree
	bool EnemyDeath();
	bool CalculateNewPatrolLocation();
	bool ArrivedAtPatrolLocation();
	bool MoveToPatrolLocation();
};