// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyControllers/BehaviourTree/SucceederNode.h"

NodeStatus SucceederNode::RunNode()
{
	GetChildNode()->RunNode();
	CurrentState = NodeStatus::SUCCESS;
	return CurrentState;
}
