// Fill out your copyright notice in the Description page of Project Settings.

#include "RecognizerUtils.h"

//define prints
#define print(text, color) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5, color, text)
#define printFString(text, fstring) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Magenta, FString::Printf(TEXT(text), fstring))


TArray<FVector2D> URecognizerUtils::Unistroke(TArray<FVector2D> points, int nrOfPoints, float squareSize) {
	
	TArray<FVector2D> newPoints = ResamplePoints(points, nrOfPoints);
	float radians = IndicativeAngle(newPoints);
	newPoints = RotatePointsBy(newPoints, -radians);
	newPoints = ScalePoints(newPoints, squareSize);
	return TranslatePoints(newPoints);
}


TArray<FVector2D> URecognizerUtils::ResamplePoints(TArray<FVector2D> points, int n) {

	//Calculate complete distance between all points
	float pathLength = 0; 
	for (int i = 1; i < points.Num(); i++) {
		pathLength += FVector2D::Distance(points[i-1], points[i]);
	}
	float I = pathLength/(n-1);

	//Create new points Array and add first input point 
	TArray<FVector2D> newPoints;
	newPoints.Add(points[0]);

	float D = 0;
	for (int i = 1; i < points.Num(); i++) {
		float d = FVector2D::Distance(points[i - 1], points[i]);
		if ((D + d) > I) {
			float qx = points[i - 1].X + ((I - D) / d) * (points[i].X - points[i - 1].X);
			float qy = points[i - 1].Y + ((I - D) / d) * (points[i].Y - points[i - 1].Y);
			FVector2D q = FVector2D(qx, qy);
			newPoints.Add(q);
			points.Insert(q, i); //insert the new q point in points, this will be the [i - 1] point in the next loop.
			D = 0;
		}
		else {
			D += d;
		}
	}
	if (newPoints.Num() == n - 1) { //rouding error can make newPoints 1 short of n, add last point in points.
		newPoints.Add(points[points.Num() - 1]);
	}
	return newPoints;
}

float URecognizerUtils::IndicativeAngle(TArray<FVector2D> points) {

	FVector2D c = Centeroid(points);
	return FMath::Atan2(c.Y - points[0].Y, c.X - points[0].X);
}


TArray<FVector2D> URecognizerUtils::RotatePointsBy(TArray<FVector2D> points, float radians) {

	FVector2D c = Centeroid(points);
	float cos = FMath::Cos(radians);
	float sin = FMath::Sin(radians);
	TArray<FVector2D> newPoints;

	//rotate every point around ceteroid
	for (int i = 0; i < points.Num(); i++) {
		float qx = (points[i].X - c.X) * cos - (points[i].Y - c.Y) * sin + c.X;
		float qy = (points[i].X - c.X) * sin + (points[i].Y - c.Y) * cos + c.Y;
		newPoints.Add(FVector2D(qx, qy));
	}
	return newPoints;
}

TArray<FVector2D> URecognizerUtils::ScalePoints(TArray<FVector2D> points, float SquareSize) {

	FBox2D B = FBox2D(points);
	TArray<FVector2D> newPoints;
	for (int i = 0; i < points.Num(); i++) {
		float qx = points[i].X * (SquareSize / B.GetSize().X);
		float qy = points[i].Y * (SquareSize / B.GetSize().Y);
		newPoints.Add(FVector2D(qx, qy));
	}
	return newPoints;
}

TArray<FVector2D> URecognizerUtils::TranslatePoints(TArray<FVector2D> points) {

	FVector2D c = Centeroid(points);
	TArray<FVector2D> newPoints;
	for (int i = 0; i < points.Num(); i++) {
		float qx = points[i].X - c.X;
		float qy = points[i].Y - c.Y;
		newPoints.Add(FVector2D(qx, qy));
	}
	return newPoints;
}

FGestureStruct URecognizerUtils::Recognize(TArray<FVector2D> points, TArray<FGestureStruct> gestureTemplates, float angleRange, float anglePrecision, float SquareSize, bool &succes, float &score, int &gestureIndex) {

	float b = INFINITY; //variablöe to save smallest distance
	gestureIndex = -1;

	float plusRange = FMath::DegreesToRadians(angleRange);
	float minusRange = FMath::DegreesToRadians(-angleRange);
	float precision = FMath::DegreesToRadians(anglePrecision);


	for (int i = 0; i < gestureTemplates.Num(); i++) {
		float d = DistanceAtBestAngle(points, gestureTemplates[i], minusRange, plusRange, precision);
		if (d < b) {
			b = d; 
			gestureIndex = i;
		}
	}
	if (gestureIndex != -1) {
		
		float diagonal = FMath::Sqrt(SquareSize * SquareSize + SquareSize * SquareSize);
		float halfDiagonal = diagonal/2;

		succes = true;
		score = (1 - b / halfDiagonal);
		return gestureTemplates[gestureIndex];
	}

	succes = false;
	return FGestureStruct();
}

 //--Help functions--

FVector2D URecognizerUtils::Centeroid(TArray<FVector2D> points) {
	
	float x = 0;
	float y = 0;
	for (int i = 0; i < points.Num(); i++) {
		x += points[i].X;
		y += points[i].Y;
	}
	return FVector2D(x/points.Num(), y/points.Num());
}

float URecognizerUtils::DistanceAtBestAngle(TArray<FVector2D> points, FGestureStruct gesture, float minusAngleRange, float plusAngleRange, float anglePrecision) {

	// Golden Ratio
	float phi = 0.5 * (-1.0 + FMath::Sqrt(5.0)); 

	float a = minusAngleRange;
	float b = plusAngleRange;

	float x1 = (phi * a) + (1 - phi) * b;
	float f1 = DistanceAtAngle(points, gesture, x1);

	float x2 = (1 - phi) * a + phi * b;
	float f2 = DistanceAtAngle(points, gesture, x2);

	while (FMath::Abs(b - a) > anglePrecision) {

		if (f1 < f2) {
			b = x2;
			x2 = x1;
			f2 = f1;
			x1 = (phi * a) + (1 - phi) * b;
			f1 = DistanceAtAngle(points, gesture, x1);
		} 
		else {
			a = x1;
			x1 = x2;
			x2 = (1 - phi) * a + phi * b;
			f2 = DistanceAtAngle(points, gesture, x2);
		}
	}

	return FMath::Min(f1, f2);
}

float URecognizerUtils::DistanceAtAngle(TArray<FVector2D> points, FGestureStruct gesture, float radians) {

	TArray<FVector2D> newpoints = RotatePointsBy(points, radians);
	return PathDistance(points, gesture.points);

}

float URecognizerUtils::PathDistance(TArray<FVector2D> points1, TArray<FVector2D> points2) {
	float d = 0;
	//point1 and points2 assumes to have the same nr of points
	for (int i = 0; i < points1.Num(); i++) {
		d += FVector2D::Distance(points1[i], points2[i]);
	}
	return d / points1.Num();
}