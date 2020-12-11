// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PickableItemDisplayData.h"
#include "UObject/NoExportTypes.h"
#include "BlueprintImplementableInterface.generated.h"

/**
 * 
 */
UINTERFACE(Blueprintable)
class UBlueprintImplementableInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IBlueprintImplementableInterface
{
	GENERATED_IINTERFACE_BODY()

    // Called by character when he picks up this item.
    // Must make it called from SERVER if you want multiplayer functions.
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category=Actions)
    void OnPickup(ACharacter* ActionCharacter);

	// Called by character when he drops this item.
	// Must make it called from SERVER if you want multiplayer functions.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category=Actions)
    void OnDrop();

	// Called by character when he queries for this item's info.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category=Status)
    FPickableItemDisplayData GetPickableInfo();

	// Called by character when he wants to determine whether this item is open for pick up.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category=Status)
    bool IsOpenForPickup();
	
};
