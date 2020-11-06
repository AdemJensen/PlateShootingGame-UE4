// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DataGeneratorLib.generated.h"

/**
 * 
 */
UCLASS()
class PLATESHOOTINGGAME_API UDataGeneratorLib final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	TArray<FString> static Adjectives;
	TArray<FString> static Nouns;

public:

	UFUNCTION(BlueprintPure)
	static FString GenerateUsername();
	
};

