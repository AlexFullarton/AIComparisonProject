// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyControllers/BehaviourTree/Decorator/FailerNode.h"

FailerNode::FailerNode(TreeNode* ChildNode) : DecoratorNode(ChildNode)
{}

void FailerNode::ChildSuccess(TreeNode* Node)
{
	ChildFailure(Node);
}
