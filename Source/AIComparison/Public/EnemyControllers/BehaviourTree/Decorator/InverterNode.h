// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../DecoratorNode.h"

class InverterNode : public DecoratorNode
{
public:
	InverterNode(TreeNode* ChildNode);
private:
	virtual void ChildSuccess(TreeNode* Node) override;
	virtual void ChildFailure(TreeNode* Node) override;
};
