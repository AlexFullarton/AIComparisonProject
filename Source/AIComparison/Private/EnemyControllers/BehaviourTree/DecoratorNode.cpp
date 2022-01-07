// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyControllers/BehaviourTree/DecoratorNode.h"

TreeNode* DecoratorNode::GetChildNode() const
{
	return ChildNode;
}

void DecoratorNode::SetChildNode(TreeNode* Node)
{
	ChildNode = Node;
}
