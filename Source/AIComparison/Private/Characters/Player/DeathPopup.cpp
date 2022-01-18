// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/DeathPopup.h"
#include "Characters/Player/PlayerCharacter.h"

void UDeathPopup::OutputPerformanceDataToFile(float highestFPS, float lowestFPS, float averageFPS, float stdDevFPS, const TArray<float>& FPSValues)
{
	// Get the programs current directory location
	std::string location = TCHAR_TO_UTF8(*FPaths::ProjectDir());
	location += "PerformanceData.json";
	// Open a file for outputting performance data
	std::ofstream OutputFile;
	OutputFile.open(location, std::ios::app);

	// create an ostringstream to act as a string buffer for the test results
	std::ostringstream osstream;

	// Output data gathered for this test cycle
	// Get time from the system
	FDateTime currentTime = FDateTime::Now();
	std::string parsedTime = TCHAR_TO_UTF8(*currentTime.ToString());
	// Get test parameters from the game instance
	UAIComparisonInstance* instance = Cast<UAIComparisonInstance>(GetWorld()->GetGameInstance());
	// Get fps data - stored in the player character
	APlayerCharacter* player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	osstream << "{\n";
	osstream << "\t\"Test\" : [\n";
	osstream << "\t\t{\n";
	osstream << "\t\t\t\"TestTime\" : \"" + parsedTime + "\",\n";
	
	// Based on enemy behaviour type, output either State Machine or Behaviour Tree
	if (instance->EnemyBehaviourType == 0)
		osstream << "\t\t\t\"BehaviourType\" : \"StateMachine\",\n";
	else
		osstream << "\t\t\t\"BehaviourType\" : \"BehaviourTree\",\n";
	// Record total amount of enemies used for the test
	osstream << "\t\t\t\"EnemyCount\" : \"" << instance->EnemyCount << "\",\n";
	// Record fps data gathered in the test
	osstream << "\t\t\t\"MinimumFPS\" : \"" << lowestFPS << "\",\n";
	osstream << "\t\t\t\"MaximumFPS\" : \"" << highestFPS << "\",\n";
	osstream << "\t\t\t\"MeanFPS\" : \"" << averageFPS << "\",\n";
	osstream << "\t\t\t\"StdDevFPS\" : \"" << stdDevFPS << "\",\n";
	osstream << "\t\t\t\"FPSData\" : [\n";
	// Record each gathered fps entry into an array structure in json format
	TArray<float> FPSData = FPSValues;
	for (float element : FPSData)
		osstream << "\t\t\t\t{ \"FramesPerSecond\" : \"" << element << "\" },\n";
	osstream << "\t\t\t]\n";
	osstream << "\t\t}\n";
	osstream << "\t]\n";
	osstream << "}\n";

	// Output contents of the string stream to the open file
	OutputFile << osstream.str();
	OutputFile.close();
}