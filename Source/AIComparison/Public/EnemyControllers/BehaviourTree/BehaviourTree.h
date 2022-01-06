// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <list>
#include <initializer_list>
#include <functional>

// Node status enum - used as a return type for tree action functions
enum NodeStatus
{
	SUCCESS,
	FAILURE,
	RUNNING
};

class BehaviourTree
{
public:
	// Abstract Class for behaviour tree nodes
	class TreeNode
	{
	public:
		virtual NodeStatus RunNode() = 0;
	};

	// Class for composite tree node - node contains a list of other sub-nodes
	class CompositeTreeNode : public TreeNode
	{
	public:
		const std::list<TreeNode*>& GetChildNodes() const;
		void AddChildNode(TreeNode* Node);
		void AddChildNodes(std::initializer_list<TreeNode*>&& Nodes);

	private:
		std::list<TreeNode*> ChildNodes;
	};

	// Class for selector tree node - only one child must succeed
	class SelectorTreeNode : public CompositeTreeNode
	{
	public:
		virtual NodeStatus RunNode() override;
	};

	// Class for sequence tree node - every child must succeed
	class SequenceTreeNode : public CompositeTreeNode
	{
	public:
		virtual NodeStatus RunNode() override;
	};

	// Class for decorator tree node - modifies the result of its child node in some way, can only have one child
	class DecoratorTreeNode : public TreeNode
	{
	private:
		TreeNode* ChildNode;

	protected:
		TreeNode* GetChildNode() const;

	public:
		void SetChildNode(TreeNode* NewChild);
	};

	// Top-level root node
	class RootNode : public DecoratorTreeNode
	{
	private:
		friend class BehaviourTree;
		virtual NodeStatus RunNode() override;
	};

	// Inverter node - if child returns success, this returns fail
	class InverterTreeNode : public DecoratorTreeNode
	{
	private:
		virtual NodeStatus RunNode() override;
	};

	// Succeeder node - will always return success - doesnt depend on childs result
	// Useful where failure is expected but the whole branch doesnt need to be abandoned
	class SucceederTreeNode : public DecoratorTreeNode
	{
	private:
		virtual NodeStatus RunNode() override;
	};

	// Failer node - will always return fail, opposite of succeeder
	class FailerTreeNode : public DecoratorTreeNode
	{
	private:
		virtual NodeStatus RunNode() override;
	};

	// Repeater node - will run  child node a given number of times/indefinitely
	class RepeaterTreeNode : public DecoratorTreeNode
	{
	private:
		int NumberOfRepeats;
		static const int INDEFINITE_REPEAT = -1;
	public:
		RepeaterTreeNode(int NumRepeats = INDEFINITE_REPEAT);
	private:
		virtual NodeStatus RunNode() override;
	};

	// Repeat until fail node - Similar to repeat, but will stop if the child fails, returning success to the parent
	class RepeatUntilFailNode : public DecoratorTreeNode
	{
	private:
		virtual NodeStatus RunNode() override;
	};

private:
	// Root node for behaviour tree instance
	RootNode* Root;

public:
	BehaviourTree();
	void SetRootNodeChild(TreeNode* Node) const;
	NodeStatus Run() const;
};

// Action class - the "leaves" of the tree
class Action : public BehaviourTree::TreeNode
{
private:
	// Pointer to the function that the action will run
	std::function<NodeStatus()> FunctionPointer;
public:
	Action(std::function<NodeStatus()> FunctionName);

private:
	virtual NodeStatus RunNode() override;
};

