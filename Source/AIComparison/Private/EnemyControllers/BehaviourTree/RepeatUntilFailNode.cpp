// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyControllers/BehaviourTree/RepeatUntilFailNode.h"

NodeStatus RepeatUntilFailNode::RunNode()
{
	// Repeat indefinitely 
	while (true)
	{
		switch (GetChildNode()->RunNode())
		{
		case NodeStatus::FAILURE:
			CurrentState = NodeStatus::SUCCESS;
			return CurrentState;
		case NodeStatus::SUCCESS:
			CurrentState = NodeStatus::SUCCESS;
		case NodeStatus::RUNNING:
			CurrentState = NodeStatus::RUNNING;
		default:
			CurrentState = NodeStatus::SUCCESS;
		}
	}
}
