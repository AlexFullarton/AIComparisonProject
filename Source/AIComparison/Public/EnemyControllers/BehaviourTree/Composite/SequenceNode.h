// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CompositeNode.h"

class SequenceNode : public CompositeNode
{
public:
	SequenceNode(std::list<TreeNode*> Nodes);
	virtual void ChildSuccess(TreeNode* Node) override;
	virtual void ChildFailure(TreeNode* Node) override;
};
