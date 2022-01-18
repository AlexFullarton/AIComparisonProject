// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "../../AIComparisonInstance.h"
#include "DeathPopup.generated.h"

UCLASS()
class AICOMPARISON_API UDeathPopup : public UUserWidget
{
	GENERATED_BODY()
		
public:
	UFUNCTION(BlueprintCallable)
	void OutputPerformanceDataToFile(float highestFPS, float lowestFPS, float averageFPS, float stdDevFPS, const TArray<float>& FPSValues);
};
