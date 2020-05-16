#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "GestureStruct.generated.h"

/**
 * 
 */
 //If you want this to appear in BP, make sure to use this instead
//USTRUCT()
 USTRUCT(BlueprintType)
struct FGestureStruct
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recognizer")
		FString key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recognizer")
		FString jsonContent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recognizer")
		TArray<FVector2D> points;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recognizer")
		float squareSizeUsed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recognizer")
		float nrOfPoints;

	//Set key
	void SetKey(const FString NewValue)
	{
		key = NewValue;
	}

	//Set content
	void SetContent(const FString NewValue)
	{
		jsonContent = NewValue;
	}

	//Set points
	void SetPoints(const TArray<FVector2D> NewValue)
	{
		points = NewValue;
	}

	//Set SquareSize
	void SetSquareSizeUsed(const float NewSquareSizeUsed)
	{
		squareSizeUsed = NewSquareSizeUsed;
	}

	//Set nrOfPoints
	void SetNrOfPoints(const float newNrOfPoints)
	{
		nrOfPoints = newNrOfPoints;
	}

	//Constructor
	FGestureStruct()
	{
		key = "";
		jsonContent = "";
	}
};