// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyControllers/BehaviourTree/SelectorNode.h"

NodeStatus SelectorNode::RunNode()
{
	// Check each child node - only one needs to successfully run to avoid branch failure
	for (TreeNode* ChildNode : GetChildNodes())
	{
		switch (ChildNode->RunNode())
		{
		case NodeStatus::FAILURE:
			continue;
		case NodeStatus::SUCCESS:
			CurrentState = NodeStatus::SUCCESS;
			return CurrentState;
		case NodeStatus::RUNNING:
			CurrentState = NodeStatus::RUNNING;
			return CurrentState;
		default:
			continue;
		}
	}
	CurrentState = NodeStatus::FAILURE;
	return CurrentState;
		
	return NodeStatus::SUCCESS;
}