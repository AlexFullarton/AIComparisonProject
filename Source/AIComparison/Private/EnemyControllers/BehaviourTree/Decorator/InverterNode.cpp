// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyControllers/BehaviourTree/Decorator/InverterNode.h"

void InverterNode::ChildSuccess(TreeNode* Node)
{
	DecoratorNode::ChildFailure(Node);
}

void InverterNode::ChildFailure(TreeNode* Node)
{
	DecoratorNode::ChildSuccess(Node);
}
