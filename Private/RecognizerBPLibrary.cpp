// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "RecognizerBPLibrary.h"
#include "Recognizer.h"


//define prints
#define print(text, color) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5, color, text)
#define printFString(text, fstring) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Magenta, FString::Printf(TEXT(text), fstring))

/**
*	
*/
URecognizerBPLibrary::URecognizerBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	
}
