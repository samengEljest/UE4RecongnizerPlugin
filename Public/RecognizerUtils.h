
#pragma once

#include "Engine.h"
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GestureStruct.h"

#include "RecognizerUtils.generated.h"


/**
 * 
 */
UCLASS()
class URecognizerUtils : public UObject
{
	GENERATED_BODY()

private:


public:

	/*
	Full 1$ unistroke recognizer
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Unistroke points", Keywords = "Recognizer unistroke"), Category = "Recognizer")
		static TArray<FVector2D> Unistroke(TArray<FVector2D> points, int nrOfPoints, float squareSize);

	/*
	 Step 1 of 1$ unistroke recognizer. Resample a points path into exactly n new points that is evenly spaced points. (n=64 in example)
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Resample recognizer points", Keywords = "Recognizer unistroke"), Category = "Recognizer")
		static TArray<FVector2D> ResamplePoints(TArray<FVector2D> points, int n);

	/*
	 Step 2 of 1$ unistroke recognizer. Find and save the indicative radians from the points’ centroid to first point.
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Indicitive Angle", Keywords = "Recognizer unistroke"), Category = "Recognizer")
		static float IndicativeAngle(TArray<FVector2D> points);
	//Then by –radians to set this angle to 0°
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Rotate points by radians", Keywords = "Recognizer unistroke"), Category = "Recognizer")
		static TArray<FVector2D> RotatePointsBy(TArray<FVector2D> points, float radians);


	/*
	 Step 3 of 1$ unistroke recognizer. Scale points so that the resulting bounding box will be of X size.  (size=250 in example)
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Scale points", Keywords = "Recognizer unistroke"), Category = "Recognizer")
		static TArray<FVector2D> ScalePoints(TArray<FVector2D> points, float SquareSize);

	 /*
	  Still part 3. Translate points to the points center
	 */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Translate points", Keywords = "Recognizer unistroke"), Category = "Recognizer")
		static TArray<FVector2D> TranslatePoints(TArray<FVector2D> points);

	/*
	  Step 4 of 1$ unistroke recognizer. 
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Recognize", Keywords = "Recognizer unistroke"), Category = "Recognizer")
		static FGestureStruct Recognize(TArray<FVector2D> points, TArray<FGestureStruct> gestureTemplates, float angleRange, float anglePrecision, float SquareSize, bool &succes, float &score, int &gestureIndex);
	/*
	 Help function, find center point of for cluster of points
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Centeroid", Keywords = "Recognizer unistroke"), Category = "Recognizer")
		static FVector2D Centeroid(TArray<FVector2D> points);

	/*
	 Golden Section Search, an algorithm that finds the minimum value in a range using the Golden Ratio phi=0.5(-1 + squrt(5)).
	 Default values:
	 angleRange +-  45.0 degrees.
	 anglePrecision 2 degrees.
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Distamce At Best Angle", Keywords = "Recognizer unistroke"), Category = "Recognizer")
		static float DistanceAtBestAngle(TArray<FVector2D> points, FGestureStruct gesture, float minusAngleRange, float plusAngleRange, float anglePrecision);

	/*
	 Rotate points and find path distance between points and gesture
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Distamce At Angle", Keywords = "Recognizer unistroke"), Category = "Recognizer")
		static float DistanceAtAngle(TArray<FVector2D> points, FGestureStruct gesture, float radians);
	
	/*
	 Find median distance between two point arrays.
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Path distance", Keywords = "Recognizer unistroke"), Category = "Recognizer")
		static float PathDistance(TArray<FVector2D> points1, TArray<FVector2D> points2);
};
