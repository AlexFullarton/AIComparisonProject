// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../TreeNode.h"
#include <initializer_list>
#include <list>
#include <iterator>

// Parallel nodes - runs all child nodes when visited by the tree
class ParallelNode : public TreeNode
{
public:
	//Constructor
	ParallelNode(std::list<TreeNode*> Nodes, RunMode Mode, RunType Type);
	
	// Returns a list of all the child nodes of this node
	const std::list<TreeNode*>& GetChildNodes() const;
	// Returns the total amount of child nodes for this node
	virtual int GetChildNodeCount() override;
	// Returns a single child node by index
	virtual TreeNode* GetChildAtIndex(int i) override;
	// Add a child node to this node
	virtual void AddChild(TreeNode* Node) override;
	// Add a list of child nodes to this node
	void AddChildNodes(std::initializer_list<TreeNode*>&& Nodes);

	// Called when a child of this node returns a status
	virtual void ChildRunning(TreeNode* RunningNode, TreeNode* ReportingNode) override;
	virtual void ChildSuccess(TreeNode* Node) override;
	virtual void ChildFailure(TreeNode* Node) override;

	// Reset this node back to its original state
	virtual void ResetNode() override;

	// Resets this nodes child nodes back to their original state
	void ResetAllChildNodes();

	// Called every time this node updates
	virtual void RunNode() override;

	virtual void Reset() override;

private:
	// List of all the child nodes of this node
	std::list<TreeNode*> ChildNodes;

	bool NoRunningChildren;
	NodeStatus LastChildResult;
	int CurrentChildIndex;

	// Resume functionality - starts or resumes all child nodes every tick
	void ResumeExecute();
	// Join functionality - child nodes execute until they complete and wont re-run until
	// this node has completed with success or failure
	void JoinExecute();

	// Sequence type - This makes this parallel node handle its children in the same way
	// as a standard sequence node - this task fails as soon as one child fails
	NodeStatus SequenceChildSuccess();
	NodeStatus SequenceChildFailure();

	// Selector type - These functions make this parallel node handle its children in the same
	// way as a standard selector node - this task fails of all children fail
	NodeStatus SelectorChildSuccess();
	NodeStatus SelectorChildFailure();

public:
	RunMode NodeRunMode;
	RunType NodeRunType;
};
