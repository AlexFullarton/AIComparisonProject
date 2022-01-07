// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyControllers/BehaviourTree/RepeaterNode.h"

RepeaterNode::RepeaterNode(int NumRepeats) : NumberOfRepeats(NumRepeats)
{}

NodeStatus RepeaterNode::RunNode()
{
	if (NumberOfRepeats == INDEFINITE_REPEAT)
	{
		// Repeat indefinitely 
		while (true)
		{
			switch (GetChildNode()->RunNode())
			{
			case NodeStatus::FAILURE:
				CurrentState = NodeStatus::FAILURE;
			case NodeStatus::SUCCESS:
				CurrentState = NodeStatus::SUCCESS;
			case NodeStatus::RUNNING:
				CurrentState = NodeStatus::RUNNING;
			default:
				CurrentState = NodeStatus::SUCCESS;
			}
		}
	}
	else
	{
		// Repeat set amount of times
		for (int i = 0; i < NumberOfRepeats - 1; i++)
		{
			switch (GetChildNode()->RunNode())
			{
			case NodeStatus::FAILURE:
				CurrentState = NodeStatus::FAILURE;
			case NodeStatus::SUCCESS:
				CurrentState = NodeStatus::SUCCESS;
			case NodeStatus::RUNNING:
				CurrentState = NodeStatus::RUNNING;
			default:
				CurrentState = NodeStatus::SUCCESS;
			}
		}
		return CurrentState;
	}
}
