// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyControllers/BehaviourTree/Composite/SequenceNode.h"

void SequenceNode::ChildSuccess(TreeNode* Node)
{
	CompositeNode::ChildSuccess(Node);
	// previous node succeeded so move on to next node
	if (++CurrentChildIndex < GetChildNodeCount())
		RunNode();
	// All children succeeded so return success
	else
		Success();
}

void SequenceNode::ChildFailure(TreeNode* Node)
{
	// If any child node of the sequence fails, then return fail
	CompositeNode::ChildFailure(Node);
	Failure();
}
