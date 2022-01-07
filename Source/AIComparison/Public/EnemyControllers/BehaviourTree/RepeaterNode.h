// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DecoratorNode.h"

// Repeater node - Repeats evaulates its child node for a set amount of iterations or indefinitely by default
class RepeaterNode : public DecoratorNode
{
public:
	RepeaterNode(int NumRepeats = INDEFINITE_REPEAT);
private:
	int NumberOfRepeats;
	static const int INDEFINITE_REPEAT = -1;
	virtual NodeStatus RunNode() override;
};
