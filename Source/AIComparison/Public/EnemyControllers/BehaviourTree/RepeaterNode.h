// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DecoratorNode.h"

// Repeater node - Repeats evaulates its child node for a set amount of iterations or indefinitely by default
class RepeaterNode : public DecoratorNode
{
public:
	RepeaterNode(int NumRepeats = INDEFINITE_REPEAT);

	virtual void StartNode() override;
	bool LoopCondition();

protected:
	// Should this decorator continue looping
	bool loop;
private:
	// How many times this loop should repeat - defaults to infinite
	int NumberOfRepeats;
	static const int INDEFINITE_REPEAT = -1;

	// Which iteration of the loop is currently running
	int count;

	virtual void RunNode() override;

	virtual void ChildRunning(TreeNode* RunningNode, TreeNode* ReportingNode) override;
	virtual void ChildSuccess(TreeNode* Node) override;
	virtual void ChildFailure(TreeNode* Node) override;
};
