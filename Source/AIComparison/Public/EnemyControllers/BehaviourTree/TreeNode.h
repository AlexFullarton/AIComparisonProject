// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// Node status enum - used as a return type for tree action functions
enum NodeStatus
{
	SUCCESS,
	FAILURE,
	RUNNING
};

// Abstract Class for behaviour tree nodes
class TreeNode
{
public:
	virtual NodeStatus RunNode() = 0;

	NodeStatus GetNodeStatus() { return CurrentState; }

protected:
	NodeStatus CurrentState;
};
