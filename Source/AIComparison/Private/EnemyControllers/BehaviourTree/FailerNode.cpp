// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyControllers/BehaviourTree/FailerNode.h"

NodeStatus FailerNode::RunNode()
{
	GetChildNode()->RunNode();
	CurrentState = NodeStatus::FAILURE;
	return CurrentState;
}
