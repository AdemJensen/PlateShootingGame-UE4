// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PickableItemDisplayData.h"
#include "PickableItem.generated.h"


UINTERFACE(Blueprintable)
class UPickableItem : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IPickableItem
{
	GENERATED_IINTERFACE_BODY()

	// Called by actor when he/she/it picks up this item.
	// Must make it called from SERVER if you want multiplayer functions.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category=Actions)
	void OnPickup(AActor* ActionActor);

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
