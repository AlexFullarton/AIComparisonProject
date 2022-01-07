// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <list>
#include <initializer_list>
#include <functional>
#include "SelectorNode.h"
#include "SequenceNode.h"
#include "InverterNode.h"
#include "SucceederNode.h"
#include "FailerNode.h"
#include "RepeaterNode.h"
#include "RepeatUntilFailNode.h"

class BehaviourTree
{
private:
	// Root node for behaviour tree instance
	SelectorNode RootNode;

public:
	BehaviourTree();

	void BuildTree();

	NodeStatus Run();
};
