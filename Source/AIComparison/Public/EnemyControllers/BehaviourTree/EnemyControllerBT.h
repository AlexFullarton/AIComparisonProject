// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyControllers/EnemyController.h"
#include <functional>
#include <memory>
#include <list>
#include "Composite/SelectorNode.h"
#include "Composite/SequenceNode.h"
#include "Composite/ParallelNode.h"
#include "Decorator/InverterNode.h"
#include "Decorator/SucceederNode.h"
#include "Decorator/FailerNode.h"
#include "Decorator/RepeaterNode.h"
#include "Decorator/RepeatUntilFailNode.h"
#include "Decorator/OnlyOnceNode.h"
#include "Action.h"
#include "EnemyControllerBT.generated.h"

UCLASS()
class AICOMPARISON_API AEnemyControllerBT : public AEnemyController, public TreeNode
{
	GENERATED_BODY()

public:
	AEnemyControllerBT();

	// Called when this controller takes controll of a pawn
	virtual void OnPossess(APawn* InPawn);
	// Called every game update
	virtual void Tick(float DeltaTime);

	// Evaulate the tree at run time
	virtual void RunNode() override;

	// Reset this node
	virtual void ResetNode() override;

	// Called when a child of the root node returns a status
	virtual void ChildRunning(TreeNode* RunningNode, TreeNode* ReportingNode) override { Running(); }
	virtual void ChildSuccess(TreeNode* Node) override { Success(); }
	virtual void ChildFailure(TreeNode* Node) override { Failure(); }

	// Add a child node to this nodes list of child nodes
	virtual void AddChild(TreeNode* Node) override {};
	// Get amount of child nodes
	virtual int GetChildNodeCount() override { return 1; }
	// Get child node at index
	virtual TreeNode* GetChildAtIndex(int i) override { return nullptr; }

	// Called in game loop to determine of the tree needs to be run
	void TickTree();

	virtual void Reset() override;
private:
	// Tree nodes that will be used to build the tree
	// Tree built from leaves up

	// To check death state
	Action CheckIfDead = Action(std::bind(&AEnemyControllerBT::EnemyDeath, this));

	// To allow enemy to retreat if health is low
	Action CheckCriticalHealth = Action(std::bind(&AEnemyControllerBT::EnemyCriticalHealth, this));
	Action GetRetreatLocation = Action(std::bind(&AEnemyControllerBT::CalculateRetreatLocation, this));
	Action Retreat = Action(std::bind(&AEnemyControllerBT::MoveToLocation, this));
	Action WaitForRetreat = Action(std::bind(&AEnemyControllerBT::CheckAtLocation, this));
	// All actions must succeed in order for enemy to retreat so use Sequence node
	SequenceNode RetreatNode = SequenceNode({ &CheckCriticalHealth, &GetRetreatLocation, &Retreat, &WaitForRetreat });
	// Enemies are restricted to only retreating the first time they are able to so must
	// parent Sequence node to only once node
	OnlyOnceNode RetreatOnce = OnlyOnceNode(&RetreatNode);

	// To allow enemy to patrol to random location in set distance
	Action CalculatePatrolDestination = Action(std::bind(&AEnemyControllerBT::CalculateNewPatrolLocation, this));
	Action MoveToPatrolDestination = Action(std::bind(&AEnemyControllerBT::MoveToLocation, this));
	Action WaitForMove = Action(std::bind(&AEnemyControllerBT::CheckAtLocation, this));
	// All actions must succeed in order for enemy to patrol so use Sequence node
	SequenceNode PatrolNode = SequenceNode({ &CalculatePatrolDestination, &MoveToPatrolDestination, &WaitForMove });

	

	// Root Node of the tree
	ParallelNode RootNode = ParallelNode({ &CheckIfDead, &RetreatOnce, &PatrolNode }, RunMode::RESUME, RunType::SELECTOR);
	
	// Action functions - passed as pointers to the action nodes/leaves of the tree
	// Death action
	NodeStatus EnemyDeath();

	// Retreat actions - Sequence parent as all need to complete
	NodeStatus EnemyCriticalHealth();
	NodeStatus CalculateRetreatLocation();
	NodeStatus MoveToLocation();
	NodeStatus CheckAtLocation();

	// Patrol actions - Sequence parent as all need to complete
	NodeStatus CalculateNewPatrolLocation();
};