// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyControllers/BehaviourTree/Decorator/RepeatUntilFailNode.h"

void RepeatUntilFailNode::StartNode()
{

}

void RepeatUntilFailNode::RunNode()
{
	loop = true;
	while (loop)
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

void RepeatUntilFailNode::ChildRunning(TreeNode* RunningNode, TreeNode* ReportingNode)
{
	DecoratorNode::ChildRunning(RunningNode, ReportingNode);
	loop = false;
}

void RepeatUntilFailNode::ChildSuccess(TreeNode* Node)
{
	loop = true;
}

void RepeatUntilFailNode::ChildFailure(TreeNode* Node)
{
	Success();
	loop = false;
}
