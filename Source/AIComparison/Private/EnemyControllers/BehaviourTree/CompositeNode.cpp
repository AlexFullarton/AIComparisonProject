// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyControllers/BehaviourTree/CompositeNode.h"

CompositeNode::CompositeNode(std::list<TreeNode*> ChildNodes) : ChildNodes(ChildNodes)
{}

const std::list<TreeNode*>& CompositeNode::GetChildNodes() const
{
	return ChildNodes;
}

int CompositeNode::GetChildNodeCount()
{
	return ChildNodes.size();
}

TreeNode* CompositeNode::GetChildAtIndex(int i)
{
	// Lists do not have random access so must cycle throuh to find element at index
	auto list_front = ChildNodes.begin();
	std::advance(list_front, i);
	return *list_front;
}

void CompositeNode::AddChild(TreeNode* Node)
{
	ChildNodes.emplace_back(Node);
}

void CompositeNode::AddChildNodes(std::initializer_list<TreeNode*>&& Nodes)
{
	for (TreeNode* Node : Nodes)
		AddChild(Node);
}

void CompositeNode::StartNode()
{
	CurrentChildIndex = 0;
	RunningChild = nullptr;
}

void CompositeNode::ChildRunning(TreeNode* RunningNode, TreeNode* ReportingNode)
{
	// Set running child
	RunningChild = RunningNode;
	// Return running status as child is running
	Running();
}

void CompositeNode::ChildSuccess(TreeNode* Node)
{
	RunningChild = nullptr;
}

void CompositeNode::ChildFailure(TreeNode* Node)
{
	RunningChild = nullptr;
}

void CompositeNode::ResetNode()
{
	TreeNode::ResetNode();
	CurrentChildIndex = 0;
	RunningChild = nullptr;
}

void CompositeNode::RunNode()
{
	// If this node has a running child node
	if (RunningChild != nullptr)
		RunningChild->RunNode();
	// Otherwise cycle through child nodes
	else
	{
		// If child index is still in range - this should never fail
		if (CurrentChildIndex < ChildNodes.size())
		{
			RunningChild = GetChildAtIndex(CurrentChildIndex);
			RunningChild->SetParentNode(this);
			RunningChild->StartNode();
			RunningChild->RunNode();
		}
	}
}

void CompositeNode::CancelRunningChildren(int index)
{
	TreeNode::CancelRunningChildren(index);
	RunningChild = nullptr;
}

void CompositeNode::Reset()
{
	CurrentChildIndex = 0;
	RunningChild = nullptr;
	ChildNodes.clear();
	TreeNode::Reset();
}
