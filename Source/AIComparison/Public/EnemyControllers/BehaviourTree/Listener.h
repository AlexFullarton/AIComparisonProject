// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TreeNode.h"

class Listener
{
public:
	virtual void StatusUpdated(TreeNode* UpdatedNode, NodeStatus OldStatus) {}

	virtual void ChildAdded(TreeNode* ChildNode, int index) {}
};
