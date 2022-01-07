// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DecoratorNode.h"

class FailerNode : public DecoratorNode
{
private:
	virtual NodeStatus RunNode() override;
};
