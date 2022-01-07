// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DecoratorNode.h"

// Repeat until fail node - Similar to repeat, but will repeat indefinitely and only stop if the child fails, returning success to the parent
class RepeatUntilFailNode : public DecoratorNode
{
public:
	virtual void StartNode() override;

protected:
	// Should this decorator continue looping
	bool loop;

private:
	virtual void RunNode() override;

	virtual void ChildRunning(TreeNode* RunningNode, TreeNode* ReportingNode) override;
	virtual void ChildSuccess(TreeNode* Node) override;
	virtual void ChildFailure(TreeNode* Node) override;
};
