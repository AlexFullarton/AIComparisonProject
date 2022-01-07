// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyControllers/BehaviourTree/TreeNode.h"
#include "EnemyControllers/BehaviourTree/EnemyControllerBT.h"

void TreeNode::SetParentNode(TreeNode* Node)
{
	ParentNode = Node;
	Tree = Node->Tree;
}

void TreeNode::Success()
{
	// Update this nodes current state
	NodeStatus OldState = CurrentState;
	CurrentState = NodeStatus::SUCCESS;

	// If the tree this node belongs to is listening for node updates
	if (Tree->Listeners.size() > 0)
		// Notify tree that this node has changed state
		Tree->NotifyStatusChanged(this, OldState);
	// If this node has a parent node i.e. is not the root node of the tree
	if (ParentNode != nullptr)
		// Update the parent node with this nodes changed state
		ParentNode->ChildSuccess(this);
}

void TreeNode::Failure()
{
	// Update this nodes current state
	NodeStatus OldState = CurrentState;
	CurrentState = NodeStatus::FAILURE;

	// If the tree this node belongs to is listening for node updates
	if (Tree->Listeners.size() > 0)
		// Notify tree that this node has changed state
		Tree->NotifyStatusChanged(this, OldState);
	// If this node has a parent node i.e. is not the root node of the tree
	if (ParentNode != nullptr)
		// Update the parent node with this nodes changed state
		ParentNode->ChildFailure(this);
}

void TreeNode::Running()
{
	// Update this nodes current state
	NodeStatus OldState = CurrentState;
	CurrentState = NodeStatus::RUNNING;

	// If the tree this node belongs to is listening for node updates
	if (Tree->Listeners.size() > 0)
		// Notify tree that this node has changed state
		Tree->NotifyStatusChanged(this, OldState);
	// If this node has a parent node i.e. is not the root node of the tree
	if (ParentNode != nullptr)
		// Update the parent node with this nodes changed state
		ParentNode->ChildRunning(this, this);
}

void TreeNode::Cancel()
{
	// Cancel any running child nodes
	CancelRunningChildren();
	NodeStatus OldState = CurrentState;
	CurrentState = NodeStatus::CANCELLED;
	if (Tree->Listeners.size() > 0)
		Tree->NotifyStatusChanged(this, OldState);
	EndNode();
}

void TreeNode::CancelRunningChildren()
{
	// Cycle through child nodes and terminate if running
	int count = GetChildNodeCount();
	for (int i = 0; i < count; i++)
	{
		TreeNode* child = GetChildAtIndex(i);
		if (child->GetNodeStatus() == NodeStatus::RUNNING)
			child->Cancel();
	}
}

void TreeNode::ResetNode()
{
	// If the node is running then terminate
	if (CurrentState == NodeStatus::RUNNING)
		Cancel();
	// Cycle through each child node and reset if running
	int count = GetChildNodeCount();
	for (int i = 0; i < count; i++)
	{
		GetChildAtIndex(i)->ResetNode();
	}
	CurrentState = NodeStatus::FRESH;
}
