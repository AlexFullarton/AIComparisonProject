// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DecoratorNode.h"

class InverterNode : public DecoratorNode
{
private:
	virtual NodeStatus RunNode() override;
};
