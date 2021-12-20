// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviourTree.h"

// Constructor
BehaviourTree::BehaviourTree() : Root(new RootNode) {}

void BehaviourTree::SetRootNodeChild(TreeNode* Node) const
{
	Root->SetChildNode(Node);
}

bool BehaviourTree::Run() const
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
bool BehaviourTree::SelectorTreeNode::RunNode()
{
	// Check each child node - only one needs to successfully run to avoid branch failure
	for (TreeNode* ChildNode : GetChildNodes())
		if (ChildNode->RunNode())
			return true;
	return false;
}

// Sequence Node
bool BehaviourTree::SequenceTreeNode::RunNode()
{
	// Check each child node - each child node must successfully run to avoid branch failure
	for (TreeNode* ChildNode : GetChildNodes())
		if (!ChildNode->RunNode())
			return false;
	return true;
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
bool BehaviourTree::RootNode::RunNode()
{
	GetChildNode()->RunNode();
}

// Inverter Node
bool BehaviourTree::InverterTreeNode::RunNode()
{
	return !GetChildNode()->RunNode();
}

// Succeeder Node
bool BehaviourTree::SucceederTreeNode::RunNode()
{
	GetChildNode()->RunNode();
	return true;
}

// Failer Node
bool BehaviourTree::FailerTreeNode::RunNode()
{
	GetChildNode()->RunNode();
	return false;
}

// Repeater Node
BehaviourTree::RepeaterTreeNode::RepeaterTreeNode(int NumRepeats) : NumberOfRepeats(NumRepeats)
{}

bool BehaviourTree::RepeaterTreeNode::RunNode()
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
bool BehaviourTree::RepeatUntilFailNode::RunNode()
{
	while(GetChildNode()->RunNode()) {}
	return true;
}

// Action Node
Action::Action(bool (*FunctionName)()) : FunctionPointer(FunctionName)
{}

bool Action::RunNode()
{
	return (*FunctionPointer)();
}
