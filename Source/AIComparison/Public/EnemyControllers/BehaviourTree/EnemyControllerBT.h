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

	virtual void allowAttack();
	virtual void allowBlock();
	virtual void allowRanged();

	virtual void SetAttackTimer();
	virtual void SetBlockTimer();
	virtual void SetRangedTimer();

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
	Action CheckPlayerSpottedSwap = Action(std::bind(&AEnemyControllerBT::PlayerSpotted, this));
	Action CheckPlayerSpottedAttack = CheckPlayerSpottedSwap;

	// Check if the controlled enemy needs to change weapon type this tick
	Action CheckMeleeStatus = Action(std::bind(&AEnemyControllerBT::GetMeleeState, this));
	Action CheckMultipleMeleeAllies = Action(std::bind(&AEnemyControllerBT::MultipleMeleeAlliesDetected, this));
	Action SwapWeaponTypeRanged = Action(std::bind(&AEnemyControllerBT::SwapWeapons, this));
	Action CheckRangedStatus = Action(std::bind(&AEnemyControllerBT::GetRangedState, this));
	Action CheckCloseToPlayer = Action(std::bind(&AEnemyControllerBT::IsEnemyCloseToPlayer, this));
	Action SwapWeaponTypeMelee = SwapWeaponTypeRanged;
	// For the weapon swaps to complete, each node must run so parent to sequence node
	SequenceNode SwapToRanged = SequenceNode({ &CheckMeleeStatus, &CheckMultipleMeleeAllies, &SwapWeaponTypeRanged });
	SequenceNode SwapToMelee = SequenceNode({ &CheckRangedStatus, &CheckCloseToPlayer, &SwapWeaponTypeMelee });

	// Only one weapon check needs to run each update to use selector to run only the first
	// successful
	SelectorNode VerifyAttackMode = SelectorNode({ &SwapToRanged, &SwapToMelee });
	// Even if no weapon swaps take place, the rest of this node needs to process so parent
	// to succeeder node that always returns true to its parent
	SequenceNode CheckPlayerAndAttackBlock = SequenceNode({ &CheckPlayerSpottedSwap,  &VerifyAttackMode });
	SucceederNode TryChangeAttackMode = SucceederNode(&CheckPlayerAndAttackBlock);

	// Does the controlled enemy need to block or attack
	Action CheckPlayerInAttackRange = Action(std::bind(&AEnemyControllerBT::CheckInAttackRange, this));
	Action CheckIfEnemyCanAttack = Action(std::bind(&AEnemyControllerBT::CanEnemyAttack, this));
	Action EnemyAttack = Action(std::bind(&AEnemyControllerBT::AttackPlayer, this));
	Action CheckPlayerInBlockRange = CheckPlayerInAttackRange;
	Action CheckIfEnemyCanBlock = Action(std::bind(&AEnemyControllerBT::CanEnemyBlock, this));
	Action EnemyBlock = Action(std::bind(&AEnemyControllerBT::BlockAttack, this));
	// For an attack to complete, each node must run successfully so use a sequence node
	// for the attack process
	SequenceNode AttackIfInRange = SequenceNode({ &CheckPlayerInAttackRange, &CheckIfEnemyCanAttack, &EnemyAttack });
	// Similaryl for block, each node must succeed so use sequence again
	SequenceNode BlockIfInRange = SequenceNode({ &CheckPlayerInBlockRange, &CheckIfEnemyCanBlock, &EnemyBlock });
	// Only one of block or attack can run at any given time so use selector node to pick
	SelectorNode BlockAndAttack = SelectorNode({&AttackIfInRange, &BlockIfInRange });
	// Even if block and attack fail, the rest of this node needs to process so parent
	// to succeeder node that always returns true to its parent
	SequenceNode CheckPlayerAndWeaponSwap = SequenceNode({ &CheckPlayerSpottedAttack, &BlockAndAttack });
	SucceederNode TryAttack = SucceederNode(&CheckPlayerAndWeaponSwap);

	// Combat sequence - see if this unit needs to change weapons, then check if it needs
	// to attack the player
	SequenceNode Combat = SequenceNode({ &TryChangeAttackMode, &TryAttack });
	
	// Does the controlled enemy need to chase the player
	Action HasSpottedPlayer = CheckPlayerSpottedAttack;
	Action MoveToPlayerLocation = Action(std::bind(&AEnemyControllerBT::MoveToPlayer, this));
	// All actions of chase node must succeed in order for enemy to chase player
	SequenceNode Chase = SequenceNode({ &HasSpottedPlayer, &MoveToPlayerLocation });

	// Patrol to random location in set distance - runs if nothing else runs
	Action CanSeePlayer = HasSpottedPlayer;
	InverterNode CantSeePlayer = InverterNode(&CanSeePlayer);
	Action WaitForMove = Action(std::bind(&AEnemyControllerBT::CheckAtLocation, this));
	Action CalculatePatrolDestination = Action(std::bind(&AEnemyControllerBT::CalculateNewPatrolLocation, this));
	Action MoveToPatrolDestination = Action(std::bind(&AEnemyControllerBT::MoveToLocation, this));
	// All actions must succeed in order for enemy to patrol so use Sequence node
	SequenceNode Patrol = SequenceNode({ &CantSeePlayer, &CalculatePatrolDestination, &MoveToPatrolDestination,  &WaitForMove });

	// Enemy is either patrolling or chasing the player so use selector node
	ParallelNode ChaseOrPatrol = ParallelNode({ &Chase, &Patrol }, RunMode::RESUME, RunType::SELECTOR);

	// Each update both the combat and movement status of the enemy needs to be evaluated
	// but only if the enemy is not retreating due to low health
	ParallelNode CombatAndMovement = ParallelNode({ &Combat, &ChaseOrPatrol }, RunMode::JOIN, RunType::SEQUENCE);
	SequenceNode OnlyIfEnemyIsntRetreating = SequenceNode({ &CheckEnemyIsntRetreating , &CombatAndMovement });

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
	NodeStatus MultipleMeleeAlliesDetected();
	NodeStatus IsEnemyCloseToPlayer();
	NodeStatus SwapWeapons();
	NodeStatus CheckInAttackRange();
	NodeStatus CanEnemyAttack();
	NodeStatus AttackPlayer();
	NodeStatus CanEnemyBlock();
	NodeStatus BlockAttack();
};