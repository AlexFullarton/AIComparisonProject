// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyControllers/BehaviourTree/RepeaterNode.h"

RepeaterNode::RepeaterNode(int NumRepeats) : NumberOfRepeats(NumRepeats)
{}

void RepeaterNode::StartNode()
{
	count = NumberOfRepeats;
}

bool RepeaterNode::LoopCondition()
{
	return (loop && count != 0);
}

void RepeaterNode::RunNode()
{
	loop = true;
	// If there are still loops left to run
	while (LoopCondition())
	{
		//  If the child node is running then continue running child node
		if (ChildNode->GetNodeStatus() == NodeStatus::RUNNING)
			ChildNode->RunNode();
		else
		{
			ChildNode->SetParentNode(this);
			ChildNode->StartNode();
			ChildNode->RunNode();
		}
	}
}

void RepeaterNode::ChildRunning(TreeNode* RunningNode, TreeNode* ReportingNode)
{
	DecoratorNode::ChildRunning(RunningNode, ReportingNode);
	loop = false;
}

void RepeaterNode::ChildSuccess(TreeNode* Node)
{
	// If there are loops remaining, decrement count
	if (count > 0)
		count--;
	// If all loops have completed, stop looping and return success
	if (count == 0)
	{
		DecoratorNode::ChildSuccess(Node);
		loop = false;
	}
	// Otherwide continue looping
	else
		loop = true;
}

void RepeaterNode::ChildFailure(TreeNode* Node)
{
	ChildSuccess(Node);
}
