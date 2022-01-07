// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TreeNode.h"

// Class for decorator tree node - modifies the result of its child node in some way, can only have one child
class DecoratorNode : public TreeNode
{
public:
	void SetChildNode(TreeNode* Node);

protected:
	TreeNode* GetChildNode() const;

private:
	TreeNode* ChildNode;
};
