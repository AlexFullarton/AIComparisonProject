// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/DeathPopup.h"
#include "Characters/Player/PlayerCharacter.h"

void UDeathPopup::OutputPerformanceDataToFile(float highestFPS, float lowestFPS, float averageFPS, float stdDevFPS, const TArray<float>& FPSValues)
{
	// Get the programs current directory location
	std::string location = TCHAR_TO_UTF8(*FPaths::ProjectDir());
	location += "PerformanceData.json";

	// create an ostringstream to act as a string buffer for the test results
	std::ostringstream osstream;

	// Attemp to open the file for reading in test data thats already there
	std::string contents = "";
	std::ifstream InputFile;
	InputFile.open(location, std::ios::in);
	if (InputFile)
	{
		// Get the contents of the file if any
		osstream << InputFile.rdbuf();
		contents = osstream.str();
		osstream.clear();
		osstream.str(std::string());
	}
	// Data gathered from file so close
	InputFile.close();
	
	// Now open file for output
	std::ofstream OutputFile;
	OutputFile.open(location, std::ios::trunc);
	if (OutputFile)
	{
		// Output data gathered for this test cycle
		// Get time from the system
		FDateTime currentTime = FDateTime::Now();
		std::string parsedTime = TCHAR_TO_UTF8(*currentTime.ToString());
		// Get test parameters from the game instance
		UAIComparisonInstance* instance = Cast<UAIComparisonInstance>(GetWorld()->GetGameInstance());
		// Get fps data - stored in the player character
		APlayerCharacter* player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

		// If file is empty, create the initial structure of the json file 
		if (contents == "")
		{
			osstream << "{\n";
			osstream << "\t\"Results\" : [\n";
		}
		// If the file already contains results, remove some characters from the end to maintain proper json structure
		else
		{
			for (int i = 0; i < 6; i++)
			{
				contents.pop_back();
			}
			contents += ",\n";
			osstream << contents;
		}

		// Output test results to file

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
		int count = FPSData.Num();
		for (int i = 0; i < count; i++)
		{
			// Each element but the last has the same structure
			if (i != count - 1)
				osstream << "\t\t\t\t{ \"FramesPerSecond\" : \"" << FPSData[i] << "\" },\n";
			// Last element has no comma
			else
				osstream << "\t\t\t\t{ \"FramesPerSecond\" : \"" << FPSData[i] << "\" }\n";
		}
		osstream << "\t\t\t]\n";
		osstream << "\t\t}\n";
		osstream << "\t]\n";
		osstream << "}\n";

		// Output contents of the string stream to the open file
		OutputFile << osstream.str();
	}
	OutputFile.close();
}