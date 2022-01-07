// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyControllers/BehaviourTree/InverterNode.h"

NodeStatus InverterNode::RunNode()
{
	switch (GetChildNode()->RunNode())
	{
	case NodeStatus::FAILURE:
		CurrentState = NodeStatus::SUCCESS;
		return CurrentState;
	case NodeStatus::SUCCESS:
		CurrentState = NodeStatus::FAILURE;
		return CurrentState;
	case NodeStatus::RUNNING:
		CurrentState = NodeStatus::RUNNING;
		return CurrentState;
	default:
		CurrentState = NodeStatus::SUCCESS;
		return CurrentState;
	}
}
