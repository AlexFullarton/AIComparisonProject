// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyControllers/BehaviourTree/Decorator/SucceederNode.h"

SucceederNode::SucceederNode(TreeNode* ChildNode) : DecoratorNode(ChildNode)
{}

void SucceederNode::ChildFailure(TreeNode* Node)
{
	ChildSuccess(Node);
}