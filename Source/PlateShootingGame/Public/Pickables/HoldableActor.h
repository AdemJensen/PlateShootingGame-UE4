// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HoldableActor.generated.h"


UINTERFACE(Blueprintable)
class UHoldableActor : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IHoldableActor
{
	GENERATED_IINTERFACE_BODY()

	// The reason why we didn't use the IPickableItem as input is because of some FEATURES that Unreal has.
	// By specifying target as an AActor, the RPC system can work normally.
	// Sometimes the networking system cannot pass a none-actor object correctly.

	// You don't have to check whether the PickableItem has implemented the IPickableItem Interface, it's guaranteed.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category=Status)
	void OnAbleToPickupItem(AActor* PickableItem);

	// You don't have to check whether the PickableItem has implemented the IPickableItem Interface, it's guaranteed.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category=Status)
    void OnLostTrackOfItem(AActor* PickableItem);
	
};