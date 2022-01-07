// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TreeNode.h"
#include <initializer_list>
#include <list>
#include <iterator>

// Class for composite nodes - can have multiple child nodes and cycle through each node when running
// Abstract - only need to instantiate derived types of this class
class CompositeNode : public TreeNode
{
public:
	// Returns a list of all the child nodes of this node
	const std::list<TreeNode*>& GetChildNodes() const;
	// Returns the total amount of child nodes for this node
	virtual int GetChildNodeCount() override;
	// Returns a single shild node by index - EXPENSIVE SO AVOID IF POSSIBLE
	virtual TreeNode* GetChildAtIndex(int i) override;
	// Add a child node to this node
	void AddChildNode(TreeNode* Node);
	// Add a list of child nodes to this node
	void AddChildNodes(std::initializer_list<TreeNode*>&& Nodes);

	// Called when this node runs for the first time
	virtual void StartNode() override;

	// Called when a child of this node returns a status
	virtual void ChildRunning(TreeNode* RunningNode, TreeNode* ReportingNode) override ;
	virtual void ChildSuccess(TreeNode* Node) override;
	virtual void ChildFailure(TreeNode* Node) override;

	// Reset this node back to its original state
	virtual void ResetNode() override;

	// Called every time this node updates
	virtual void RunNode() override;

protected:
	// Child node that is in the running state -  null if none
	TreeNode* RunningChild;

	// Index to the child node currently being visited
	int CurrentChildIndex;

	// Called when this node is cancelled, cancels all running children
	virtual void CancelRunningChildren() override;
private:
	std::list<TreeNode*> ChildNodes;
};
