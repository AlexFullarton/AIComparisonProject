// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyControllers/BehaviourTree/DecoratorNode.h"

TreeNode* DecoratorNode::GetChildNode() const
{
	return ChildNode;
}

void DecoratorNode::SetChildNode(TreeNode* Node)
{
	ChildNode = Node;
}

int DecoratorNode::GetChildNodeCount()
{
	return ChildNode == nullptr ? 0 : 1;
}

void DecoratorNode::RunNode()
{
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

void DecoratorNode::ChildRunning(TreeNode* RunningNode, TreeNode* ReportingNode)
{
	Running();
}

void DecoratorNode::ChildSuccess(TreeNode* Node)
{
	Success();
}

void DecoratorNode::ChildFailure(TreeNode* Node)
{
	Failure();
}
