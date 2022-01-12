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
	// Tree built from "leaves" up, Action == leaf, XNode = branch

	// Check if the controlled enemy needs to die this tick
	Action CheckIfDead = Action(std::bind(&AEnemyControllerBT::EnemyDeath, this));

	// Check if the controlled enemy needs to retreat this tick (each enemy can do this one time)
	Action CheckCriticalHealth = Action(std::bind(&AEnemyControllerBT::EnemyCriticalHealth, this));
	Action GetRetreatLocation = Action(std::bind(&AEnemyControllerBT::CalculateRetreatLocation, this));
	Action Retreat = Action(std::bind(&AEnemyControllerBT::MoveToLocation, this));
	Action WaitForRetreat = Action(std::bind(&AEnemyControllerBT::EnemyHasRetreated, this));
	// All actions must succeed in order for enemy to retreat so use Sequence node
	SequenceNode RetreatNode = SequenceNode({ &CheckCriticalHealth, &GetRetreatLocation, &Retreat, &WaitForRetreat });
	// Enemies are restricted to only retreating the first time they are able to so must
	// parent Sequence node to only once node
	OnlyOnceNode RetreatOnce = OnlyOnceNode(&RetreatNode);

	// Check if this enemy is currently retreating
	Action CheckIfEnemyRetreating = Action(std::bind(&AEnemyControllerBT::IsEnemyRetreating, this));
	InverterNode CheckEnemyIsntRetreating = InverterNode(&CheckIfEnemyRetreating);

	// Check if the player has been spotted by this enemy
	Action CheckPlayerSpotted = Action(std::bind(&AEnemyControllerBT::PlayerSpotted, this));
	
	// Check if the controlled enemy needs to change weapon type this tick
	Action CheckMeleeStatus = Action(std::bind(&AEnemyControllerBT::GetMeleeState, this));
	Action CheckRangedStatus = Action(std::bind(&AEnemyControllerBT::GetRangedState, this));
	Action CheckMultipleAllies = Action(std::bind(&AEnemyControllerBT::MultipleAlliesDetected, this));
	Action CheckCloseToPlayer = Action(std::bind(&AEnemyControllerBT::IsEnemyCloseToPlayer, this));
	Action SwapWeaponTypeRanged = Action(std::bind(&AEnemyControllerBT::SwapWeapons, this));
	Action SwapWeaponTypeMelee = SwapWeaponTypeRanged;

	SequenceNode SwapToRanged = SequenceNode({ &CheckMeleeStatus, &CheckMultipleAllies, &SwapWeaponTypeRanged });
	SequenceNode SwapToMelee = SequenceNode({ &CheckRangedStatus, &CheckCloseToPlayer, &SwapWeaponTypeMelee });

	SelectorNode VerifyAttackMode = SelectorNode({ &SwapToRanged, &SwapToMelee });
	SucceederNode TryChangeAttackMode = SucceederNode(&VerifyAttackMode);

	// Does the controlled enemy need to block or attack
	Action CheckPlayerInAttackRange = Action(std::bind(&AEnemyControllerBT::CheckInAttackRange, this));
	Action CheckIfEnemyCanAttack = Action(std::bind(&AEnemyControllerBT::CanEnemyAttack, this));
	Action EnemyAttack = Action(std::bind(&AEnemyControllerBT::AttackPlayer, this));
	Action CheckPlayerInBlockRange = CheckPlayerInAttackRange;
	Action CheckIfEnemyCanBlock = Action(std::bind(&AEnemyControllerBT::CanEnemyBlock, this));
	Action EnemyBlock = Action(std::bind(&AEnemyControllerBT::BlockAttack, this));
	SequenceNode AttackIfInRange = SequenceNode({ &CheckPlayerInAttackRange, &CheckIfEnemyCanAttack, &EnemyAttack });
	SequenceNode BlockIfInRange = SequenceNode({ &CheckPlayerInBlockRange, &CheckIfEnemyCanBlock, &EnemyBlock });
	ParallelNode BlockAndAttack = ParallelNode({&AttackIfInRange, &BlockIfInRange }, RunMode::RESUME, RunType::SELECTOR);
	SucceederNode TryCombat = SucceederNode(&BlockAndAttack);

	// Does the controlled enemy need to chase the player
	Action MoveToPlayerLocation = Action(std::bind(&AEnemyControllerBT::MoveToPlayer, this));
	// All actions of chase node must succeed in order for enemy to chase player
	SequenceNode Combat = SequenceNode({ &CheckPlayerSpotted, &TryChangeAttackMode, &TryCombat, &MoveToPlayerLocation });

	// patrol to random location in set distance
	Action CalculatePatrolDestination = Action(std::bind(&AEnemyControllerBT::CalculateNewPatrolLocation, this));
	Action MoveToPatrolDestination = Action(std::bind(&AEnemyControllerBT::MoveToLocation, this));
	Action WaitForMove = Action(std::bind(&AEnemyControllerBT::CheckAtLocation, this));
	// All actions must succeed in order for enemy to patrol so use Sequence node
	SequenceNode Patrol = SequenceNode({ &CalculatePatrolDestination, &MoveToPatrolDestination, &WaitForMove });

	ParallelNode FightOrPatrol = ParallelNode({ &Combat, &Patrol }, RunMode::RESUME, RunType::SELECTOR);
	SequenceNode OnlyIfEnemyIsntRetreating = SequenceNode({ &CheckEnemyIsntRetreating , &FightOrPatrol });

	// Root Node of the tree
	ParallelNode RootNode = ParallelNode({ &CheckIfDead, &RetreatOnce, &OnlyIfEnemyIsntRetreating }, RunMode::RESUME, RunType::SELECTOR);
	

	// Action function pointers - passed as a parameter on creation of an action node
	// The respective function will be called when the action node is run
	// Must have a return type of NodeStatus for compatability with the behaviour tree
	// Death action
	NodeStatus EnemyDeath();

	// Patrol, Chase & Retreat actions
	NodeStatus EnemyCriticalHealth();
	NodeStatus CalculateRetreatLocation();
	NodeStatus MoveToLocation();
	NodeStatus CheckAtLocation();
	NodeStatus PlayerSpotted();
	NodeStatus MoveToPlayer();
	NodeStatus CalculateNewPatrolLocation();
	NodeStatus IsEnemyRetreating();
	NodeStatus EnemyHasRetreated();

	// Combat actions
	NodeStatus GetMeleeState();
	NodeStatus GetRangedState();
	NodeStatus MultipleAlliesDetected();
	NodeStatus IsEnemyCloseToPlayer();
	NodeStatus SwapWeapons();
	NodeStatus CheckInAttackRange();
	NodeStatus CanEnemyAttack();
	NodeStatus AttackPlayer();
	NodeStatus CanEnemyBlock();
	NodeStatus BlockAttack();
};