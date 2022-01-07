// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TreeNode.h"
#include <functional>

// Action class - the "leaves" of the tree where functionality is implemented
class Action : public TreeNode
{
public:
	// Constructor - takes a  function pointer to whatever method this node will run
	// Method must have a return type of NodeStatus
	Action(std::function<NodeStatus()> Function);

private:
	std::function<NodeStatus()> FunctionPointer;

	virtual NodeStatus RunNode() override;
};
