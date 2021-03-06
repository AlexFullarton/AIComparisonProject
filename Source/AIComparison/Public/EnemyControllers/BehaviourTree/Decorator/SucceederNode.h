// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../DecoratorNode.h"

class SucceederNode : public DecoratorNode
{
public:
	SucceederNode(TreeNode* ChildNode);
private:
	virtual void ChildFailure(TreeNode* Node) override;
};
