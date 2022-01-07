// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TreeNode.h"
#include <initializer_list>
#include <list>

class CompositeNode : public TreeNode
{
public:
	const std::list<TreeNode*>& GetChildNodes() const;
	void AddChildNode(TreeNode* Node);
	void AddChildNodes(std::initializer_list<TreeNode*>&& Nodes);

private:
	std::list<TreeNode*> ChildNodes;
};
