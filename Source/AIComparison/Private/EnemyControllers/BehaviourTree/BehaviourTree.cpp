// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyControllers/BehaviourTree/BehaviourTree.h"

// Constructor
BehaviourTree::BehaviourTree() : Root(new RootNode) {}

void BehaviourTree::SetRootNodeChild(TreeNode* Node) const
{
	Root->SetChildNode(Node);
}

NodeStatus BehaviourTree::Run() const
{
	return Root->RunNode();
}

// Composite Node
const std::list<BehaviourTree::TreeNode*>& BehaviourTree::CompositeTreeNode::GetChildNodes() const
{
	return ChildNodes;
}

void BehaviourTree::CompositeTreeNode::AddChildNode(TreeNode* Node)
{
	ChildNodes.emplace_back(Node);
}

void BehaviourTree::CompositeTreeNode::AddChildNodes(std::initializer_list<TreeNode*>&& Nodes)
{
	for (TreeNode* Node : Nodes)
		AddChildNode(Node);
}

// Selector Node
NodeStatus BehaviourTree::SelectorTreeNode::RunNode()
{
	// Check each child node - only one needs to successfully run to avoid branch failure
	for (TreeNode* ChildNode : GetChildNodes())
		return ChildNode->RunNode();
	return NodeStatus::SUCCESS;
}

// Sequence Node
NodeStatus BehaviourTree::SequenceTreeNode::RunNode()
{
	// Check each child node - each child node must successfully run to avoid branch failure
	for (TreeNode* ChildNode : GetChildNodes())
		return ChildNode->RunNode();
	return NodeStatus::FAILURE;
}

// Decorator Node
BehaviourTree::TreeNode* BehaviourTree::DecoratorTreeNode::GetChildNode() const
{
	return ChildNode;
}

void BehaviourTree::DecoratorTreeNode::SetChildNode(TreeNode* NewChild)
{
	ChildNode = NewChild;
}

// Root Node
NodeStatus BehaviourTree::RootNode::RunNode()
{
	return GetChildNode()->RunNode();
}

// Inverter Node
NodeStatus BehaviourTree::InverterTreeNode::RunNode()
{
	if (GetChildNode()->RunNode() == NodeStatus::FAILURE)
		return NodeStatus::SUCCESS;
	if (GetChildNode()->RunNode() == NodeStatus::SUCCESS)
		return NodeStatus::FAILURE;
	return GetChildNode()->RunNode();
}

// Succeeder Node
NodeStatus BehaviourTree::SucceederTreeNode::RunNode()
{
	GetChildNode()->RunNode();
	return NodeStatus::SUCCESS;
}

// Failer Node
NodeStatus BehaviourTree::FailerTreeNode::RunNode()
{
	GetChildNode()->RunNode();
	return NodeStatus::FAILURE;
}

// Repeater Node
BehaviourTree::RepeaterTreeNode::RepeaterTreeNode(int NumRepeats) : NumberOfRepeats(NumRepeats)
{}

NodeStatus BehaviourTree::RepeaterTreeNode::RunNode()
{
	if (NumberOfRepeats == INDEFINITE_REPEAT)
		// Repeat indefinitely
		while (true)
			GetChildNode()->RunNode();
	else
	{
		for (int Index = 0; Index < NumberOfRepeats - 1; Index++)
			GetChildNode()->RunNode();
		return GetChildNode()->RunNode();
	}
}

// Repeat until fail Node
NodeStatus BehaviourTree::RepeatUntilFailNode::RunNode()
{
	while(GetChildNode()->RunNode()) {}
	return NodeStatus::SUCCESS;
}

// Action Node
Action::Action(std::function<NodeStatus()> FunctionName) : FunctionPointer(FunctionName)
{}

NodeStatus Action::RunNode()
{
	return FunctionPointer();
}
