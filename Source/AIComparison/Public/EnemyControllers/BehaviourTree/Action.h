// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TreeNode.h"
#include <functional>

// Action class - the "leaves" of the tree where functionality is implemented
class Action : public TreeNode
{
public:
	// Constructor - takes a  function pointer to whatever method this node will run
	// Method must have a return type of NodeStatus
	Action(std::function<NodeStatus()> Function);

	// Do the action of this node
	NodeStatus ExecuteNode();

	// Called when a child of this node returns a status
	virtual void ChildSuccess(TreeNode* SucceededNode) final {};
	virtual void ChildFailure(TreeNode* FailedNode) final {};
	virtual void ChildRunning(TreeNode* RunningNode, TreeNode* ReportingNode) final {};

	// Leaf node cannot have children
	virtual int GetChildNodeCount() { return 0; }
	virtual TreeNode* GetChildAtIndex(int i) { return nullptr; }
	virtual void AddChild(TreeNode* Node) override {}

private:
	std::function<NodeStatus()> FunctionPointer;

	virtual void RunNode() final;
};
