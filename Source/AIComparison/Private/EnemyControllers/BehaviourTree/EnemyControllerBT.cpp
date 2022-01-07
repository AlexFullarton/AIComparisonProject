// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyControllers/BehaviourTree/EnemyControllerBT.h"
#include "Characters/Enemy/EnemyCharacter.h"

AEnemyControllerBT::AEnemyControllerBT()
{
	Tree = this;
}

void AEnemyControllerBT::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Create the structure of the behaviour tree for this instance of the enemy controller
	BuildTree();
}

void AEnemyControllerBT::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Run behaviour tree only if controller is possessing a pawn
	if (controlledEnemy)
		TickTree();
}

void AEnemyControllerBT::BuildTree()
{
	// Form the structure of the tree starting with the leaf nodes and working up to the root
	
	// First: Does the enemy need to enter its death state due to low health?
	
	// Next: Has the enemy reached its previous patrol destination?
	// If it has, calculate a new destination and move to that instead
	PatrolNode.AddChildNodes({ &CalculatePatrolDestination, &MoveToPatrolDestination, &WaitForMove });

	// Add to root node to complete the tree
	RootNode.AddChildNodes({ &CheckIfDead, &PatrolNode });
}

void AEnemyControllerBT::TickTree()
{
	// If the root node returned a status of running last update
	if (RootNode.GetNodeStatus() == NodeStatus::RUNNING)
		RootNode.RunNode();
	else
	{ 
		// If the root node is not running, start it
		RootNode.SetParentNode(this);
		RootNode.StartNode();
		RootNode.RunNode();
	}
}

void AEnemyControllerBT::RunNode()
{
	//RootNode.RunNode();
}

NodeStatus AEnemyControllerBT::EnemyDeath()
{
	if (isDead)
	{
		SetActorTickEnabled(false);
		return NodeStatus::SUCCESS;
	}
	return NodeStatus::FAILURE;
}

NodeStatus AEnemyControllerBT::CalculateNewPatrolLocation()
{
	UNavigationSystemV1* navSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (navSystem)
	{
		// Find a new location for the enemy to patrol to
		if (navSystem->GetRandomReachablePointInRadius(controlledEnemy->GetActorLocation(), searchRadius, destination))
			return NodeStatus::SUCCESS;
	}
	return NodeStatus::FAILURE;
}

NodeStatus AEnemyControllerBT::CheckAtPatrolLocation()
{
	// If the enemy is still moving to its destination
	if (GetMoveStatus() == EPathFollowingStatus::Moving)
		return NodeStatus::RUNNING;
	else
	// If the enemy has arrived at its stored destination
	if (GetMoveStatus() != EPathFollowingStatus::Moving)
		return NodeStatus::SUCCESS;
	return NodeStatus::FAILURE;
}

NodeStatus AEnemyControllerBT::MoveToPatrolLocation()
{
	// Patrol to the new destination
	if (MoveToLocation(destination.Location, tolerance))
		return NodeStatus::SUCCESS;
	return NodeStatus::FAILURE;
}

void AEnemyControllerBT::ResetNode()
{
	TreeNode::ResetNode();
	// or Tree = this; ???
	SetParentNode(this);
}

void AEnemyControllerBT::NotifyStatusChanged(TreeNode* Node, NodeStatus status)
{
	for (Listener* listener : Listeners)
	{
		listener->StatusUpdated(Node, status);
	}
}

void AEnemyControllerBT::NotifyChildAdded(TreeNode* Node, int index)
{
	for (Listener* listener : Listeners)
	{
		listener->ChildAdded(Node, index);
	}
}



