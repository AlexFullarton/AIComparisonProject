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

	// If this node has a parent node i.e. is not the root node of the tree
	if (ParentNode != nullptr)
		// Update the parent node with this nodes changed state
		ParentNode->ChildRunning(this, this);
}

void TreeNode::Cancel()
{
	// Cancel any running child nodes
	CancelRunningChildren(0);
	NodeStatus OldState = CurrentState;
	CurrentState = NodeStatus::CANCELLED;
	EndNode();
}

void TreeNode::CancelRunningChildren(int index)
{
	// Cycle through child nodes and terminate if running
	int count = GetChildNodeCount();
	for (int i = index; i < count; i++)
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

void TreeNode::Reset()
{
	ParentNode = nullptr;
	CurrentState = NodeStatus::FRESH;
	Tree = nullptr;
}
