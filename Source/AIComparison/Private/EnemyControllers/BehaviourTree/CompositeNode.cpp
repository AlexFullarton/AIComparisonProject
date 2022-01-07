// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyControllers/BehaviourTree/CompositeNode.h"

const std::list<TreeNode*>& CompositeNode::GetChildNodes() const
{
	return ChildNodes;
}

void CompositeNode::AddChildNode(TreeNode* Node)
{
	ChildNodes.emplace_back(Node);
}

void CompositeNode::AddChildNodes(std::initializer_list<TreeNode*>&& Nodes)
{
	for (TreeNode* Node : Nodes)
		AddChildNode(Node);
}