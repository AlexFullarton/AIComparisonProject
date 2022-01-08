// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class AEnemyControllerBT;

// Node status enum - used as a return type for tree action functions
enum NodeStatus
{
	FRESH,
	SUCCESS,
	FAILURE,
	RUNNING,
	CANCELLED
};

// Parallel Node type enums
enum RunMode
{
	RESUME,
	JOIN
};

enum RunType
{
	SEQUENCE,
	SELECTOR
};

// Abstract Class for behaviour tree nodes
class TreeNode
{
public:	
	// Returns node status
	NodeStatus GetNodeStatus() { return CurrentState; }

	// Set parent node
	virtual void SetParentNode(TreeNode* Node);

	// Called just before this node is run for the first time
	virtual void StartNode() {};

	// Called upon completion of this node
	virtual void EndNode() {};

	// Called to inform ParentNode of this nodes state
	void Success();
	void Failure();
	void Running();

	// Terminates this node - can only be called while this node is running
	void Cancel();

	// Reset this node back to its original state
	virtual void ResetNode();

	virtual void Reset();

	// Pure virtual functions - functionality will differ between node types
	// Will be implemented for each type of node in derived classes
	// Called every time this node updates
	virtual void RunNode() = 0;

	// Called when a child of this node returns a status
	virtual void ChildSuccess(TreeNode* SucceededNode) = 0;
	virtual void ChildFailure(TreeNode* FailedNode) = 0;
	virtual void ChildRunning(TreeNode* RunningNode, TreeNode* ReportingNode) = 0;

	// Add a child node to this nodes list of child nodes
	virtual void AddChild(TreeNode* Node) = 0;
	// Gets the amount of child nodes this node has
	virtual int GetChildNodeCount() = 0;
	// Gets the child node at the given index
	virtual TreeNode* GetChildAtIndex(int i) = 0;

protected:
	// Node status for evaluating tree at run time
	NodeStatus CurrentState;

	// This nodes parent node - only null for root node
	TreeNode* ParentNode;

	// Behaviour tree that this node belongs to
	AEnemyControllerBT* Tree;

	// Terminates any running child nodes
	virtual void CancelRunningChildren(int index);
};
