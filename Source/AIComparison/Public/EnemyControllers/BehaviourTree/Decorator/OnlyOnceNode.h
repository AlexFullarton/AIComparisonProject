// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../DecoratorNode.h"

class  OnlyOnceNode : public DecoratorNode
{
public:
	OnlyOnceNode(TreeNode* ChildNode);

	virtual void RunNode() override;

	virtual void ChildSuccess(TreeNode* Node) override;

	virtual void Reset() override;
private:
	bool ChildNodeHasSucceeded = false;
};
