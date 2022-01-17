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

NodeStatus AEnemyControllerBT::EnemyCriticalHealth()
{
	if (IsCriticalHealth())
	{
		isRetreating = true;
		return NodeStatus::SUCCESS;
	}
	else
		return NodeStatus::FAILURE;
}

NodeStatus AEnemyControllerBT::CalculateRetreatLocation()
{
	// When retreating to a random location, set character speed to be high
	Cast<UCharacterMovementComponent>(controlledEnemy->GetMovementComponent())->MaxWalkSpeed = chaseSpeed;
	UNavigationSystemV1* navSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (navSystem)
	{
		// Find a new location for the enemy to retreat to
		if (navSystem->GetRandomReachablePointInRadius(controlledEnemy->GetActorLocation(), searchRadius, destination))
			return NodeStatus::SUCCESS;
	}
	return NodeStatus::FAILURE;
}

NodeStatus AEnemyControllerBT::MoveToLocation()
{
	// Patrol to the new destination
	if (AEnemyController::MoveToLocation(destination.Location, tolerance))
		return NodeStatus::SUCCESS;
	return NodeStatus::FAILURE;
}

NodeStatus AEnemyControllerBT::CheckAtLocation()
{
	FVector2D pawnLocation2D = FVector2D(pawnLocation.X, pawnLocation.Y);
	FVector2D destination2D = FVector2D(destination.Location.X, destination.Location.Y);
	float distanceToLocation =  (pawnLocation2D - destination2D).Size();
	if (distanceToLocation <= (tolerance + controlledEnemy->GetCapsuleColliderRadius() + 5.0f))
		return NodeStatus::SUCCESS;
	else
		return NodeStatus::RUNNING;
}

NodeStatus AEnemyControllerBT::PlayerSpotted()
{
	if (sensedPlayer != nullptr && !isRetreating)
		return NodeStatus::SUCCESS;
	else
		return NodeStatus::FAILURE;
}

NodeStatus AEnemyControllerBT::IsEnemyRetreating()
{
	if (isRetreating)
		return NodeStatus::SUCCESS;
	else
		return NodeStatus::FAILURE;
}

NodeStatus AEnemyControllerBT::MoveToPlayer()
{
	// When chasing the player, set the movement speed to be high
	Cast<UCharacterMovementComponent>(controlledEnemy->GetMovementComponent())->MaxWalkSpeed = chaseSpeed;
	UNavigationSystemV1* navSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (navSystem)
	{
		// Move the enemy to the player character actor, using the reference
		// to the detected player actor and acceptance radius
		if (isMelee)
			MoveToActor(sensedPlayer, meleeTolerance);
		else
			MoveToActor(sensedPlayer, rangedTolerance);
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

NodeStatus AEnemyControllerBT::EnemyHasRetreated()
{
	// If the enemy is still moving to its destination
	if (GetMoveStatus() == EPathFollowingStatus::Moving)
		return NodeStatus::RUNNING;
	else
		// If the enemy has arrived at its stored destination
		if (GetMoveStatus() != EPathFollowingStatus::Moving)
		{
			isRetreating = false;
			return NodeStatus::SUCCESS;
		}
	return NodeStatus::FAILURE;
}

NodeStatus AEnemyControllerBT::GetMeleeState()
{
	if (isMelee)
		return NodeStatus::SUCCESS;
	else
		return NodeStatus::FAILURE;
}

NodeStatus AEnemyControllerBT::GetRangedState()
{
	if (isRanged)
		return NodeStatus::SUCCESS;
	else
		return NodeStatus::FAILURE;
}

NodeStatus AEnemyControllerBT::MultipleMeleeAlliesDetected()
{
	int meleeAlliesDetected = 0;
	for (AActor* actor : sensedFriendlies)
	{
		// Double check that the sensed actor is in fact another enemy
		AEnemyCharacter* enemy_ally = Cast<AEnemyCharacter>(actor);
		// Count the total amount of melee allies detected by this pawn
		if (enemy_ally->isMelee)
			meleeAlliesDetected++;
	}
	// Return success when 3 or more melee allies are detected
	return meleeAlliesDetected >= 3 ? NodeStatus::SUCCESS : NodeStatus::FAILURE;
}

NodeStatus AEnemyControllerBT::IsEnemyCloseToPlayer()
{
	if (getDistanceToPlayer() <= meleeAttackRange)
		return NodeStatus::SUCCESS;
	else
		return NodeStatus::FAILURE;
}

NodeStatus AEnemyControllerBT::SwapWeapons()
{
	Super::SwapWeapons();
	return NodeStatus::SUCCESS;
}

NodeStatus AEnemyControllerBT::CheckInAttackRange()
{
	if (isMelee && getDistanceToPlayer() < meleeAttackRange)
		return NodeStatus::SUCCESS;
	else if (isRanged && getDistanceToPlayer() < rangedAttackRange)
		return NodeStatus::SUCCESS;
	return NodeStatus::FAILURE;
}

NodeStatus AEnemyControllerBT::CanEnemyAttack()
{
	//if (isMelee && !IsEnemyAttacking())
	if (isMelee && !IsEnemyAttacking() && IsAttackAllowed())
	{
		disallowBlock();
		return NodeStatus::SUCCESS;
	}
	//else if (isRanged && !IsEnemyAttacking())
	else if (isRanged && !IsEnemyAttacking() && IsRangedAllowed())
		return NodeStatus::SUCCESS;
	return NodeStatus::FAILURE;
}

NodeStatus AEnemyControllerBT::AttackPlayer()
{
	// If the block timer has not already been started
	// When timer ends, the controlled enemy stops being locked out of the block action
	if (!blockTimerRunning)
	{
		SetBlockTimer();
		blockTimerRunning = true;
	}
	RotateToFacePlayer();
	if (isMelee)
	{
		Attack();
		return NodeStatus::SUCCESS;
	}
	else if (isRanged)
	{
		AttackRanged();
		return NodeStatus::SUCCESS;
	}
	return NodeStatus::FAILURE;
}

NodeStatus AEnemyControllerBT::CanEnemyBlock()
{
	if (isMelee && IsBlockAllowed())
	{
		disallowAttack();
		return NodeStatus::SUCCESS;
	}
	return NodeStatus::FAILURE;
}

NodeStatus AEnemyControllerBT::BlockAttack()
{
	// If the attack timer has not already been started
	// When the timer ends, the controlled enemy stops being locked out of the attack action
	if (!attackTimerRunning)
	{
		SetAttackTimer();
		attackTimerRunning = true;
	}
	Block();
	return NodeStatus::SUCCESS;
}

void AEnemyControllerBT::allowAttack()
{
	attackTimerRunning = false;
	StopBlocking();
	Super::allowAttack();
}

void AEnemyControllerBT::allowBlock()
{
	blockTimerRunning = false;
	Super::allowBlock();
}

void AEnemyControllerBT::allowRanged()
{
	rangedTimerRunning = false;
	Super::allowRanged();
}

void AEnemyControllerBT::SetAttackTimer()
{
	float rate = FMath::RandRange(attackRate, attackRate + 1.5f);
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AEnemyControllerBT::allowAttack, rate, false);
}

void AEnemyControllerBT::SetBlockTimer()
{
	float rate = FMath::RandRange(1.5, 3.0f);
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AEnemyControllerBT::allowBlock, rate, false);
}

void AEnemyControllerBT::SetRangedTimer()
{

}


