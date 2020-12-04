// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PickableItemDisplayData.h"
#include "UObject/NoExportTypes.h"
#include "TestiveInterface.generated.h"

/**
 * 
 */
UINTERFACE(Blueprintable)
class UTestiveInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class ITestiveInterface
{
	GENERATED_IINTERFACE_BODY()

    // Called by character when he picks up this item.
    // Must make it called from SERVER if you want multiplayer functions.
    UFUNCTION(Category=Actions)
    virtual void OnPickup(ACharacter* ActionCharacter);

	// Called by character when he drops this item.
	// Must make it called from SERVER if you want multiplayer functions.
	UFUNCTION(Category=Actions)
    virtual void OnDrop();

	// Called by character when he queries for this item's info.
	UFUNCTION(Category=Status)
    virtual FPickableItemDisplayData GetPickableInfo();

	// Called by character when he wants to determine whether this item is open for pick up.
	UFUNCTION(Category=Status)
    virtual bool IsOpenForPickup();
	
};
