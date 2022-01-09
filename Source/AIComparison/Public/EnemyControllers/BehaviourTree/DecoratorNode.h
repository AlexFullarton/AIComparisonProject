// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TreeNode.h"

// Class for decorator tree node - modifies the result of its child node in some way, can only have one child
// Abstract - only need to instantiate derived types of this class
class DecoratorNode : public TreeNode
{
public:
	// Constructor
	DecoratorNode(TreeNode* ChildNode);

	// Return the amount of child nodes this node has - either 0 or 1 for decorator
	virtual int GetChildNodeCount() override;
	// Add a child to this node if there isnt one already
	virtual void AddChild(TreeNode* Node) override;
	// Gets the child node at the given index
	virtual TreeNode* GetChildAtIndex(int i) override;

	virtual void RunNode() override;

	virtual void ChildRunning(TreeNode* RunningNode, TreeNode* ReportingNode) override ;
	virtual void ChildSuccess(TreeNode* Node) override;
	virtual void ChildFailure(TreeNode* Node) override;

	virtual void Reset() override;
protected:
	// Decorator nodes have only one child
	TreeNode* ChildNode;	
};
