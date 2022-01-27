// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyControllers/BehaviourTree/Composite/ParallelNode.h"

ParallelNode::ParallelNode(std::list<TreeNode*> Nodes, RunMode Mode, RunType Type) : ChildNodes(Nodes), NodeRunMode(Mode), NodeRunType(Type)
{}

const std::list<TreeNode*>& ParallelNode::GetChildNodes() const
{
	return ChildNodes;
}

int ParallelNode::GetChildNodeCount()
{
	return ChildNodes.size();
}

TreeNode* ParallelNode::GetChildAtIndex(int i)
{
	auto list_front = ChildNodes.begin();
	std::advance(list_front, i);
	return *list_front;
}

void ParallelNode::AddChild(TreeNode* Node)
{
	ChildNodes.emplace_back(Node);
}

void ParallelNode::AddChildNodes(std::initializer_list<TreeNode*>&& Nodes)
{
	for (TreeNode* Node : Nodes)
		AddChild(Node);
}

void ParallelNode::ChildRunning(TreeNode* RunningNode, TreeNode* ReportingNode)
{
	NoRunningChildren = false;
}

void ParallelNode::ChildSuccess(TreeNode* Node)
{
	if (NodeRunType == RunType::SELECTOR)
		LastChildResult = SelectorChildSuccess();
	else
		LastChildResult = SequenceChildSuccess();
}

void ParallelNode::ChildFailure(TreeNode* Node)
{
	if (NodeRunType == RunType::SELECTOR)
		LastChildResult = SelectorChildFailure();
	else
		LastChildResult = SequenceChildFailure();
}

void ParallelNode::ResetNode()
{
	TreeNode::ResetNode();
	NoRunningChildren = true;
}

void ParallelNode::ResetAllChildNodes()
{
	// Cycle through each child node of this node
	for (TreeNode* ChildNode : ChildNodes)
	{
		ChildNode->ResetNode();
	}
}

void ParallelNode::RunNode()
{
	if (NodeRunMode == RunMode::RESUME)
		ResumeExecute();
	else
		JoinExecute();
}

void ParallelNode::Reset()
{
	NodeRunMode = RunMode::RESUME;
	NodeRunType = RunType::SEQUENCE;
	NoRunningChildren = true;
	LastChildResult = NodeStatus::FRESH;
	CurrentChildIndex = 0;
	ChildNodes.clear();
	TreeNode::Reset();
}

void ParallelNode::ResumeExecute()
{
	NoRunningChildren = true;
	LastChildResult = NodeStatus::FRESH;
	// For each child node of this node
	for (CurrentChildIndex = 0; CurrentChildIndex < GetChildNodeCount(); CurrentChildIndex++)
	{
		// Get the child node currently being evaulated
		TreeNode* ChildNode = GetChildAtIndex(CurrentChildIndex);
		// If the current child node is running, continue running it
		if (ChildNode->GetNodeStatus() == NodeStatus::RUNNING)
			ChildNode->RunNode();
		// Otherwise start running the child node
		else
		{
			ChildNode->SetParentNode(this);
			ChildNode->StartNode();
			ChildNode->RunNode();
		}
		// If the current child has finished running
		if (LastChildResult == NodeStatus::SUCCESS || LastChildResult == NodeStatus::FAILURE)
		{
			int index;
			// If none of the previous children are running, only need to cancel from this node onwards
			if (NoRunningChildren)
				index = CurrentChildIndex + 1;
			// If a previous child node is running then cancel all child nodes
			else
				index = 0;
			// Cancel any remaining running children of this node
			CancelRunningChildren(index);
			if (LastChildResult == NodeStatus::SUCCESS)
				Success();
			else
				Failure();
			return;
		}
	}
	Running();
}

void ParallelNode::JoinExecute()
{
	NoRunningChildren = true;
	LastChildResult = NodeStatus::FRESH;
	for (CurrentChildIndex = 0; CurrentChildIndex < GetChildNodeCount(); CurrentChildIndex++)
	{
		// Get the child node currently being evaulated
		TreeNode* ChildNode = GetChildAtIndex(CurrentChildIndex);
		// Only run child nodes if they havent completed - running nodes re run until completion
		// and nodes that havent started yet are started
		switch (ChildNode->GetNodeStatus())
		{
		case NodeStatus::RUNNING:
			ChildNode->RunNode();
			break;
		case NodeStatus::FAILURE:
		case NodeStatus::SUCCESS:
			break;
		case NodeStatus::FRESH:
			ChildNode->SetParentNode(this);
			ChildNode->StartNode();
			ChildNode->RunNode();
		}
		// If the current child has finished running
		if (LastChildResult == NodeStatus::SUCCESS || LastChildResult == NodeStatus::FAILURE)
		{
			int index;
			// If none of the previous children are running, only need to cancel from this node onwards
			if (NoRunningChildren)
				index = CurrentChildIndex + 1;
			// If a previous child node is running then cancel all child nodes
			else
				index = 0;
			// Cancel any remaining running children of this node
			CancelRunningChildren(index);
			// Reset all of the cancelled child nodes
			ResetAllChildNodes();
			if (LastChildResult == NodeStatus::SUCCESS)
				Success();
			else
				Failure();
			return;
		}
	}
	Running();
}

NodeStatus ParallelNode::SequenceChildSuccess()
{
	if (NodeRunMode == RunMode::JOIN)
	{
		// Find node status of last child node - For a sequence node all children must succeed
		NodeStatus LastChildNodeStatus = GetChildAtIndex(GetChildNodeCount() - 1)->GetNodeStatus();
		bool LastChildSucceeded = LastChildNodeStatus == NodeStatus::SUCCESS;
		// If this is the last child node to succeed sequence node is finished
		if (NoRunningChildren && LastChildSucceeded)
			return NodeStatus::SUCCESS;
		// If any child nodes arent successfully completed then keep iterating until they are
		else
			return NodeStatus::RUNNING;
	}
	else
	{
		// If this is the last child node to succeed sequence node if complete
		bool AllChildNodesEvaluated = CurrentChildIndex == GetChildNodeCount() - 1;
		if (NoRunningChildren && AllChildNodesEvaluated)
			return NodeStatus::SUCCESS;
		// If any child nodes arent complete keep running until they succeed
		else
			return NodeStatus::RUNNING;
	}
}

NodeStatus ParallelNode::SequenceChildFailure()
{
	// If any child node of a sequence fails so does the sequence node
	return NodeStatus::FAILURE;
}

NodeStatus ParallelNode::SelectorChildSuccess()
{
	// If a child node of a selector succeeds, the selector node completes
	return NodeStatus::SUCCESS;
}

NodeStatus ParallelNode::SelectorChildFailure()
{
	// If this is the final child node to complete, the selector fails as none of its children succeeded
	bool AllRunningNodesEvaluated = CurrentChildIndex == GetChildNodeCount() - 1;
	if (NoRunningChildren && AllRunningNodesEvaluated)
		return NodeStatus::FAILURE;
	else
		return NodeStatus::RUNNING;
}