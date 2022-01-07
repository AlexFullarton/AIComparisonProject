// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyControllers/BehaviourTree/SequenceNode.h"

NodeStatus SequenceNode::RunNode()
{
	bool ChildNodeRunning = false;
	// Check each child node - any child failure results in failure of the entire node,
	// and success is only returned when all children return success
	for (TreeNode* ChildNode : GetChildNodes())
	{
		switch (ChildNode->RunNode())
		{
		case NodeStatus::FAILURE:
			CurrentState = NodeStatus::FAILURE;
			return CurrentState;
		case NodeStatus::SUCCESS:
			continue;
		case NodeStatus::RUNNING:
			ChildNodeRunning = true;
			continue;
		default:
			CurrentState = NodeStatus::SUCCESS;
			return CurrentState;
		}
	}
	// If any of the child nodes returned running then return running for this node,
	// otherwise all children have succeeded so return success
	CurrentState = ChildNodeRunning ? NodeStatus::RUNNING : NodeStatus::SUCCESS;
	return CurrentState;
}
