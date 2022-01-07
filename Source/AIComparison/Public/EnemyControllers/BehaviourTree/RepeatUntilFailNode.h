// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DecoratorNode.h"

// Repeat until fail node - Similar to repeat, but will repeat indefinitely and only stop if the child fails, returning success to the parent
class RepeatUntilFailNode : public DecoratorNode
{
private:
	virtual NodeStatus RunNode() override;
};
