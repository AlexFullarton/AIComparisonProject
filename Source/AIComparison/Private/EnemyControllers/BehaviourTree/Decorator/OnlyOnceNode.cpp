// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyControllers/BehaviourTree/Decorator/OnlyOnceNode.h"

OnlyOnceNode::OnlyOnceNode(TreeNode* ChildNode) : DecoratorNode(ChildNode)
{}

void OnlyOnceNode::RunNode()
{
	// If this nodes child node has already successfully run, then return failure
	if (ChildNodeHasSucceeded)
		DecoratorNode::ChildFailure(ChildNode);
	// If this nodes child node is running, continue to run the node
	if (ChildNode->GetNodeStatus() == NodeStatus::RUNNING)
		ChildNode->RunNode();
	// Child node is not running, so start
	else
	{
		ChildNode->SetParentNode(this);
		ChildNode->StartNode();
		ChildNode->RunNode();
	}
}

void OnlyOnceNode::ChildSuccess(TreeNode* Node)
{
	ChildNodeHasSucceeded = true;
	DecoratorNode::ChildSuccess(Node);
}

void OnlyOnceNode::Reset()
{
	ChildNodeHasSucceeded = false;
	DecoratorNode::Reset();
}