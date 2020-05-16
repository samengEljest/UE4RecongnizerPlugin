#pragma once

#include "FileHandler.h"

//define prints
#define print(text, color) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5, color, text)
#define printFString(text, fstring) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Magenta, FString::Printf(TEXT(text), fstring))

/**
*
*/
UFileHandler::UFileHandler(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{}

/**
*	Write String to json file.
*/
bool UFileHandler::WriteStringToJsonFile(FString contents, FString directoryName, FString fileName, bool overWrite, bool logToScreen)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	//Find full dirctory
	FString fullDirPath = FPaths::ProjectDir() + directoryName;
	FString fullFilePath = fullDirPath + "/" + fileName + ".json";

	//Dir Exists?
	if (!PlatformFile.DirectoryExists(*fullDirPath))
	{

		//still could not make directory?
		if (!PlatformFile.CreateDirectory(*fullDirPath))
		{
			if (logToScreen)
				printFString("Directory creaton faild %s", *fullDirPath);
			UE_LOG(LogTemp, Error, TEXT("Directory creaton faild %s"), *fullDirPath);

			//Could not make the specified directory
			return false;
		}
	}

	//No over-writing?
	if (!overWrite)
	{
		//Check if file exists already
		if (PlatformFile.FileExists(*fullFilePath))
		{
			if (logToScreen)
				printFString("File already exists %s", *fullFilePath);
			UE_LOG(LogTemp, Warning, TEXT("File already exists %s"), *fullFilePath);

			//no overwriting
			return false;
		}
	}

	//Log file created
	if (logToScreen)
		printFString("Recogicion file saved: %s", *fullFilePath);
	UE_LOG(LogTemp, Log, TEXT("Recogicion file saved: %s"), *fullFilePath);

	//Save string to file
	bool success = FFileHelper::SaveStringToFile(contents, *fullFilePath);

	//Save success
	if (logToScreen)
		printFString("Save success: %s", success ? "True" : "False");
	UE_LOG(LogTemp, Log, TEXT("Save success: %s"), success ? "True" : "False");

	//Create file and set contents
	return success;
}

bool UFileHandler::WriteStructsToJsonFile(bool logToScreen, TArray<FGestureStruct> gesturesStruct, FString directoryName, FString fileName, bool overWrite)
{
	FString contentFile = CreateNewModifiedJsonFile(logToScreen, gesturesStruct);
	return WriteStringToJsonFile(contentFile, directoryName, fileName, overWrite, logToScreen);
}

/**
*	Read from json file.
*/
FString UFileHandler::ReadJsonFile(FString directoryName, FString fileName, bool logToScreen)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	//Find full dirctory
	FString fullDirPath = FPaths::ProjectDir() + directoryName;
	FString fullFilePath = fullDirPath + "/" + fileName + ".json";
	FString fileContents;

	//Check if directory exists
	if (!PlatformFile.DirectoryExists(*fullDirPath))
	{
		if (logToScreen)
			printFString("Directory could not be found %s", *fullDirPath);
		UE_LOG(LogTemp, Error, TEXT("Directory could not be found %s"), *fullDirPath);

		//Could not make the specified directory
		return "";
	}

	//Check if file exists
	if (!PlatformFile.FileExists(*fullFilePath))
	{
		if (logToScreen)
			printFString("File was not found %s", *fullFilePath);
		UE_LOG(LogTemp, Log, TEXT("File was not found %s"), *fullFilePath);

		return "";
	}

	//Load file inte fileContents string
	FFileHelper::LoadFileToString(fileContents, *fullFilePath);

	if (logToScreen)
		printFString("File contens: %s", *fileContents);
	UE_LOG(LogTemp, Warning, TEXT("File contens: %s"), *fileContents);

	//Return file contents
	return fileContents;
}

/**
*	Create a json structure string.
*/
FString UFileHandler::CreateGestgureString(FString key, TArray<FVector2D> points, float squareSize, bool logToScreen)
{
	//"key": [
	FString newJsonString = FString("\"" + key + "\": [\n");

	for (int i = 0; i < points.Num(); i++)
	{
		//	{"X":0, "Y":0},
		newJsonString.Append(FString("	{\"X\":" + FString::SanitizeFloat(points[i].X) + ", \"Y\":" + FString::SanitizeFloat(points[i].Y) + "}"));
		if (i < points.Num()) {
			//,
			newJsonString.Append(",\n");
		}
	}
	// nr of points and square size
	newJsonString.Append(FString("	{\"nrOfPoints\":" + FString::FromInt(points.Num()) + ", \"squareSize\":" + FString::SanitizeFloat(squareSize) + "}"));
	//]
	newJsonString.Append("]");

	if (logToScreen)
		printFString("Json content: \n%s", *newJsonString);
	UE_LOG(LogTemp, Log, TEXT("Json content: \n%s"), *newJsonString);

	return newJsonString;
}

/**
*	Create a json structure struct.
*/
FGestureStruct UFileHandler::CreateGestgureStruct(FString key, TArray<FVector2D> points, float squareSize, FString gestureJson)
{
	FGestureStruct newStruct;

	newStruct.jsonContent = gestureJson;
	newStruct.points = points;
	newStruct.key = key;
	newStruct.nrOfPoints = points.Num();
	newStruct.squareSizeUsed = squareSize;

	return newStruct;
}

/**
*	Create a json structure string and struct.
*/
FGestureStruct UFileHandler::CreateGestgureStringAndStruct(FString key, TArray<FVector2D> points, float squareSize, bool logToScreen)
{
	FString newJsonContent = CreateGestgureString(key, points, squareSize, logToScreen);
	FGestureStruct newGesture = CreateGestgureStruct(key, points, squareSize, newJsonContent);

	return newGesture;
}

/*
* Create a new modified json file
*/
FString UFileHandler::CreateNewModifiedJsonFile(bool logToScreen, TArray<FGestureStruct> gesturesStruct)
{
	FString newModifiedJson = "{\n";

	//Print message and return if is invalid
	if (gesturesStruct.Num() == 0)
	{
		if (logToScreen)
			print("Gestures array invalid", FColor::Red);
		UE_LOG(LogTemp, Error, TEXT("Gestures array invalid"));
		return "";
	}

	//Find all gesture content
	for (int i = 0; i < gesturesStruct.Num(); i++)
	{
		//Print message if gesture i has no content
		if (gesturesStruct[i].jsonContent == "") {
			FString iString = FString::FromInt(i);
			if (logToScreen)
				printFString("Gestures array index %s has no content string", *iString);
			UE_LOG(LogTemp, Error, TEXT("Gestures array index %s has no content string"), *iString);
		}

		//get content from gestures[i]
		newModifiedJson.Append(gesturesStruct[i].jsonContent);
		if (i < gesturesStruct.Num() - 1) {
			//,
			newModifiedJson.Append(",\n");
		}
		else {
			//
			newModifiedJson.Append("\n");
		}
	}
	newModifiedJson.Append("}");

	return newModifiedJson;
	
}

//Create gesture array from file contents
TArray<FGestureStruct> UFileHandler::CreateGesturesFromFileContents(FString fileContent, bool logToScreen)
{
	//Array with gesturs to return
	TArray<FGestureStruct> gestures;

	//Create JsonObject and jsonReader
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(fileContent);

	//Try to deserialize so we get file contents to JsonObject
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
	{
		//Loop all gestures
		for (auto& Elem : JsonObject->Values)
		{
			//get key (gesture name)
			FString key = Elem.Key;

			//get points array
			TArray<FVector2D> points;
			TArray<TSharedPtr<FJsonValue>> objArray = JsonObject->GetArrayField(key);

			//loop all points. Minus one for nrOfPoints and squareSize
			for (int32 i = 0; i < objArray.Num() - 1; i++)
			{
				//Get X and Y values from Json object
				TSharedPtr<FJsonObject> point = objArray[i]->AsObject();
				double X = point->GetNumberField("X");
				double Y = point->GetNumberField("Y");

				//Create 2D vector to store point values
				FVector2D point2D;
				point2D.X = X;
				point2D.Y = Y;

				//Save point in points array
				points.Add(point2D);
			}

			TSharedPtr<FJsonObject> data = objArray.Last()->AsObject();
			int nrOfPoints = data->GetIntegerField("nrOfPoints");
			float squareSize = data->GetNumberField("squareSize");

			//Generate gesture structs and add to gesture array that will be rentund
			FGestureStruct readGesture = CreateGestgureStruct(key, points, squareSize, CreateGestgureString(key, points, squareSize, logToScreen));
			gestures.Add(readGesture);
		}
	}
	else
	{
		if (logToScreen)
			print("Could not deserialize json string", FColor::Red);
		UE_LOG(LogTemp, Error, TEXT("Could not deserialize json string"));
	}

	//Log nr of gesturs found in file contents
	FString nrGestures = FString::FromInt(gestures.Num());
	if (logToScreen)
		printFString("%s Gestures created from file contents", *nrGestures);
	UE_LOG(LogTemp, Log, TEXT("%s Gestures created from file contents"), *nrGestures);

	//Return gestures
	return gestures;
}
