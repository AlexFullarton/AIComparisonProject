// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyControllers/BehaviourTree/DecoratorNode.h"

DecoratorNode::DecoratorNode(TreeNode* ChildNode) : ChildNode(ChildNode)
{}

void DecoratorNode::AddChild(TreeNode* Node)
{
	if (ChildNode != nullptr)
		ChildNode = Node;
}

int DecoratorNode::GetChildNodeCount()
{
	return ChildNode == nullptr ? 0 : 1;
}

TreeNode* DecoratorNode::GetChildAtIndex(int i)
{
	return (i == 0) ? ChildNode : nullptr;
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

void DecoratorNode::Reset()
{
	ChildNode = nullptr;
	TreeNode::Reset();
}
