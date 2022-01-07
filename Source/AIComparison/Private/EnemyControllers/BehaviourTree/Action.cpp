// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyControllers/BehaviourTree/Action.h"

Action::Action(std::function<NodeStatus()> Func) : FunctionPointer(Func)
{}

NodeStatus Action::ExecuteNode()
{ 
	// Do action here - MUST RETURN NODE STATUS ENUM OF SOME TYPE
	return FunctionPointer();
}

void Action::RunNode()
{
	// Execute the action of this leaf node
	NodeStatus Status = ExecuteNode();
	// Return NodeStatus dependent on result of leaf execution
	switch (Status)
	{
	case NodeStatus::SUCCESS:
		Success();
		return;
	case NodeStatus::FAILURE:
		Failure();
		return;
	case NodeStatus::RUNNING:
		Running();
		return;
	default:
		Failure();
		return;
	}
}
