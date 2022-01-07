// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyControllers/EnemyController.h"
#include <functional>
#include <memory>
#include <list>
#include "SelectorNode.h"
#include "SequenceNode.h"
#include "InverterNode.h"
#include "SucceederNode.h"
#include "FailerNode.h"
#include "RepeaterNode.h"
#include "RepeatUntilFailNode.h"
#include "Action.h"
#include "Listener.h"
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

	// Create the structure of the behaviour tree
	void BuildTree();

	// Evaulate the tree at run time
	virtual void RunNode() override;

	// Reset this node
	virtual void ResetNode() override;

	// Called when a child of the root node returns a status
	virtual void ChildRunning(TreeNode* RunningNode, TreeNode* ReportingNode) override { Running(); }
	virtual void ChildSuccess(TreeNode* Node) override { Success(); }
	virtual void ChildFailure(TreeNode* Node) override { Failure(); }

	// Get amount of child nodes
	virtual int GetChildNodeCount() override { return 1; }
	// Get child node at index
	virtual TreeNode* GetChildAtIndex(int i) override { return &RootNode; }

	// Called in game loop to determine of the tree needs to be run
	void TickTree();

	// List of Listeners for the behaviour tree
	std::list<Listener*> Listeners;

	// Listener controller functions
	void AddListener(Listener* l) { Listeners.emplace_back(l); }
	void RemoveListener(Listener* l) { Listeners.remove(l); }
	void RemoveAllListeners() { Listeners.clear(); }
	void NotifyStatusChanged(TreeNode* Node, NodeStatus status);
	void NotifyChildAdded(TreeNode* Node, int index);

private:
	// Tree nodes that will be used to build the tree
	// Each tree must have a root node
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