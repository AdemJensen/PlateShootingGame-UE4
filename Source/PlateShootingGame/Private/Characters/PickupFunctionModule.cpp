// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PickupFunctionModule.h"

#include "Components/VerticalBoxSlot.h"
#include "Characters/PickableInfoWidget.h"
#include "Pickables/PickableItem.h"

// Sets default values for this component's properties
UPickupFunctionModule::UPickupFunctionModule()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPickupFunctionModule::BeginPlay()
{
	Super::BeginPlay();

	// Checks if the Display Label Class has implemented the standard interface.
	if (PickableDisplayLabelClass != nullptr &&
        !PickableDisplayLabelClass->Implements<UPickableInfoWidget>())
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected widget class for [PickableDisplayLabel] did not implement the "
                    "standard interface (UPickableInfoWidget), information might be incomplete on widget!"));
	}
}


// Called every frame
void UPickupFunctionModule::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UPickupFunctionModule::AddItemToList(AActor* ItemAbleToPickup)
{
	AController* OwnerController = GetOwner()->GetInstigatorController();
	if (PickableWidgetList.Contains(ItemAbleToPickup) || !OwnerController->IsLocalPlayerController())
	{
		return false;
	}
	UUserWidget* SubWidget = CreateWidget(Cast<APlayerController>(OwnerController), PickableDisplayLabelClass);
	if (PickableDisplayLabelClass != nullptr &&
        !PickableDisplayLabelClass->Implements<UPickableInfoWidget>())
	{
		// Configure the display data for created widget if it has implemented the standard interface.
		Cast<IPickableInfoWidget>(SubWidget)->Execute_ApplyPickableInfo(SubWidget,
			Cast<IPickableItem>(ItemAbleToPickup)->Execute_GetPickableInfo(ItemAbleToPickup));
	}
	PickableWidgetList.Add(ItemAbleToPickup, SubWidget);
	DisplayBox->AddChildToVerticalBox(SubWidget)->SetPadding(FMargin(0, 0, 0, 20));
	return true;
}

bool UPickupFunctionModule::RemoveItemFromList(AActor* ItemLostTrack)
{
	if (!PickableWidgetList.Contains(ItemLostTrack))
	{
		return false;
	}
	DisplayBox->RemoveChild(PickableWidgetList[ItemLostTrack]);
	PickableWidgetList.Remove(ItemLostTrack);
	return true;
}

bool UPickupFunctionModule::PickupItem(AActor* ItemToPickup)
{
	if (PickableWidgetList.Contains(ItemToPickup))
	{
		OnPickupItemEvent.Broadcast(ItemToPickup);
		return true;
	}
	return false;
}

bool UPickupFunctionModule::PickupFirstItem()
{
	if (PickableWidgetList.Num() == 0) return false;
	TArray<AActor*> ActorsAbleToPick;
	PickableWidgetList.GetKeys(ActorsAbleToPick);
	return PickupItem(ActorsAbleToPick[0]);
}

