#pragma once

#include "Engine.h"
#include "Json.h"
#include "Core.h"
#include "GestureStruct.h"
#include "kismet/blueprintfunctionlibrary.h"
#include "FileHandler.generated.h"


UCLASS()
class  UFileHandler : public UObject
{
	GENERATED_UCLASS_BODY()

private:
	

public:

	//Write String to file
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Write String to json file", Keywords = "Recognizer json"), Category = "Recognizer")
		static bool WriteStringToJsonFile(FString contents, FString directoryName, FString fileName, bool overWrite, bool logToScreen);

	//Write Gestures to file
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Write Gestures to json file", Keywords = "Recognizer json"), Category = "Recognizer")
		static bool WriteStructsToJsonFile(bool logToScreen, TArray<FGestureStruct> gesturesStruct, FString directoryName, FString fileName, bool overWrite);

	//Read from file
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Read from Json", Keywords = "Recognizer json"), Category = "Recognizer")
		static FString ReadJsonFile(FString directoryName, FString fileName, bool logToScreen);

	//Create gesture string from key and points
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Create Json gesture string", Keywords = "Recognizer json"), Category = "Recognizer")
		static FString CreateGestgureString(FString key, TArray<FVector2D> points, float squareSize, bool logToScreen);

	//Create gesture string from key and points and gesture string
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Create Json gesture string and struct", Keywords = "Recognizer json"), Category = "Recognizer")
		static FGestureStruct CreateGestgureStringAndStruct(FString key, TArray<FVector2D> points, float squareSize, bool logToScreen);

	//Create gesture string from key and points and gesture string
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Create Json gesture struct", Keywords = "Recognicer json"), Category = "Recognizer")
		static FGestureStruct CreateGestgureStruct(FString key, TArray<FVector2D> points, float squareSize, FString gestureJson);

	//Create a new modified json file
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Create Full Json string", Keywords = "Recognizer json"), Category = "Recognizer")
		static FString CreateNewModifiedJsonFile(bool logToScreen, TArray<FGestureStruct> gesturesStruct);

	//Create Gestures from file contents
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Create Gestures Form Json file content", Keywords = "Recognizer json"), Category = "Recognizer")
		static TArray<FGestureStruct> CreateGesturesFromFileContents(FString fileContent, bool logToScreen);
};