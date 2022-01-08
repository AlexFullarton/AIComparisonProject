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
}

void AEnemyControllerBT::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Run behaviour tree only if controller is possessing a pawn
	if (controlledEnemy)
		TickTree();
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
{}

void AEnemyControllerBT::ResetNode()
{
	TreeNode::ResetNode();
	// or Tree = this; ???
	SetParentNode(this);
}

void AEnemyControllerBT::Reset()
{
	ParallelNode NewRoot({}, RunMode::RESUME, RunType::SEQUENCE);
	RootNode = NewRoot;
	TreeNode::Reset();
}

NodeStatus AEnemyControllerBT::EnemyDeath()
{
	if (isDead)
	{
		EnemyKilled();
		SetActorTickEnabled(false);
		return NodeStatus::SUCCESS;
	}
	return NodeStatus::FAILURE;
}

NodeStatus AEnemyControllerBT::CalculateNewPatrolLocation()
{
	// When patrolling to a random location, set character speed to be low
	Cast<UCharacterMovementComponent>(controlledEnemy->GetMovementComponent())->MaxWalkSpeed = patrolSpeed;
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



