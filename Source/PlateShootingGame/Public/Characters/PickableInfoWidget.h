// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PickableItemDisplayData.h"

#include "PickableInfoWidget.generated.h"


UINTERFACE(Blueprintable)
class UPickableInfoWidget : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IPickableInfoWidget
{
	GENERATED_IINTERFACE_BODY()

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category=Settings)
	void ApplyPickableInfo(FPickableItemDisplayData PickableInfo);
	
};