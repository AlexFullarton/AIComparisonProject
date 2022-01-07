// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyControllers/BehaviourTree/Action.h"

Action::Action(std::function<NodeStatus()> Func) : FunctionPointer(Func)
{}

NodeStatus Action::RunNode()
{
	switch (FunctionPointer())
	{
	case NodeStatus::SUCCESS:
		CurrentState = NodeStatus::SUCCESS;
		return CurrentState;
	case NodeStatus::FAILURE:
		CurrentState = NodeStatus::FAILURE;
		return CurrentState;
	case NodeStatus::RUNNING:
		CurrentState = NodeStatus::RUNNING;
		return CurrentState;
	default:
		CurrentState = NodeStatus::FAILURE;
		return CurrentState;
	}
}
