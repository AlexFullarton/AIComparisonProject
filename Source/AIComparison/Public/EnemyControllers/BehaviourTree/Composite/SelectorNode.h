// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CompositeNode.h"

class SelectorNode : public CompositeNode
{
public:
	SelectorNode(std::list<TreeNode*> Nodes);
	// If any child succeeds, then this node also succeeds
	virtual void ChildSuccess(TreeNode* Node) override;
	virtual void ChildFailure(TreeNode* Node) override;
};
