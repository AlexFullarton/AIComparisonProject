// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyControllers/BehaviourTree/FailerNode.h"

void FailerNode::ChildSuccess(TreeNode* Node)
{
	ChildFailure(Node);
}
