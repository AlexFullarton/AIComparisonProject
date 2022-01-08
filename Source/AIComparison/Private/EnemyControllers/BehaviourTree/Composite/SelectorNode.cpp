// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyControllers/BehaviourTree/Composite/SelectorNode.h"

SelectorNode::SelectorNode(std::list<TreeNode*> Nodes) : CompositeNode(Nodes)
{}

void SelectorNode::ChildSuccess(TreeNode* Node)
{
	// On completion of any child with success, return success
	CompositeNode::ChildSuccess(Node);
	Success();
}

void SelectorNode::ChildFailure(TreeNode* Node)
{
	CompositeNode::ChildFailure(Node);
	// previous child failed so increment child index
	if (++CurrentChildIndex < GetChildNodeCount())
		RunNode();
	else
		// If all child nodes ran without success
		Failure();
}
