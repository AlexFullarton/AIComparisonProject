// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyControllers/BehaviourTree/SucceederNode.h"

void SucceederNode::ChildFailure(TreeNode* Node)
{
	ChildSuccess(Node);
}