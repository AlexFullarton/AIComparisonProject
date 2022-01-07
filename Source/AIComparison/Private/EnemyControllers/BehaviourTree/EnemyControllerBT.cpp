// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyControllers/BehaviourTree/EnemyControllerBT.h"
#include "Characters/Enemy/EnemyCharacter.h"

AEnemyControllerBT::AEnemyControllerBT()
{}

void AEnemyControllerBT::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Create the structure of the behaviour tree for this instance of the enemy controller
	//behaviourTree.SetRootNodeChild(&selector_nodes[0]);
	//selector_nodes[0].AddChildNode(&repeater_node);
	//repeater_node.SetChildNode(&sequence_nodes[0]);
	//sequence_nodes[0].AddChildNodes({ &actions[0], &actions[1], &actions[2] });
}

void AEnemyControllerBT::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Run behaviour tree only if controller is possessing a pawn
	if (controlledEnemy)
		behaviourTree.Run();
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

NodeStatus AEnemyControllerBT::ArrivedAtPatrolLocation()
{
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

NodeStatus AEnemyControllerBT::EnemyDeath()
{
	if (isDead)
		return NodeStatus::SUCCESS;
	return NodeStatus::FAILURE;
}

