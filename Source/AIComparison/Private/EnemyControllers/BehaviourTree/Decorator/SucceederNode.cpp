// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyControllers/BehaviourTree/Decorator/SucceederNode.h"

void SucceederNode::ChildFailure(TreeNode* Node)
{
	ChildSuccess(Node);
}