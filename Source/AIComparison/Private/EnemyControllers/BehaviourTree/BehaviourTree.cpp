// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyControllers/BehaviourTree/BehaviourTree.h"

// Constructor
BehaviourTree::BehaviourTree()
{}

NodeStatus BehaviourTree::Run()
{
	return RootNode.RunNode();
}