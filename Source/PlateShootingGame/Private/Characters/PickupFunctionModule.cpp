// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PickupFunctionModule.h"


#include "GeneratedCodeHelpers.h"
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
	if (PickableDisplayLabelClass != nullptr)
	{
		auto ImplementedInterfaces = PickableDisplayLabelClass->Interfaces;
		for (int i = 0; i < ImplementedInterfaces.Num(); i++)
		{
			if (ImplementedInterfaces[i].Class->IsChildOf(UPickableInfoWidget::StaticClass()))
			{
				bDisplayLabelClassProperlySetup = true;
				break;
			}
		}
		if (!bDisplayLabelClassProperlySetup)
		{
			UE_LOG(LogTemp, Warning, TEXT("Selected widget class for [PickableDisplayLabel] did not implement the "
                                "standard interface (IPickableInfoWidget), information might be incomplete on widget!"));
		}
	}
	// if (!IsValid(DisplayBox))
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("The Pickable display box is invalid, pickable info will be incomplete. Please "
	// 		"configure the PickupFunctionModule->DisplayBox in your character BP properly!"));
	// }
}

// Called every frame
void UPickupFunctionModule::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Now, after the DisplayBox has been properly configured, the missed items can finally be placed.
	if (bShouldScanPickable && IsValid(DisplayBox) && IsValid(PickableDisplayLabelClass))
	{
		TArray<AActor*> OverlappingActors;
		GetOwner()->GetOverlappingActors(OverlappingActors, UPickableItem::StaticClass());
		for (int i = 0; i < OverlappingActors.Num(); i++)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *OverlappingActors[i]->GetName());
			AddItemToList(OverlappingActors[i]);
		}
		bShouldScanPickable = false;
	}
}

bool UPickupFunctionModule::AddItemToList(AActor* ItemAbleToPickup)
{
	if (!IsValid(DisplayBox) || !IsValid(PickableDisplayLabelClass))
	{
		return false;
	}
	if (PickableItems.AddUnique(ItemAbleToPickup) != INDEX_NONE)
	{
		AController* OwnerController = GetOwner()->GetInstigatorController();
        if (!IsValid(OwnerController) || !OwnerController->IsLocalPlayerController())
        {
        	return true;
        }
		UUserWidget* SubWidget = CreateWidget(Cast<APlayerController>(OwnerController), PickableDisplayLabelClass);
        if (IsValid(PickableDisplayLabelClass) && bDisplayLabelClassProperlySetup)
        {
        	// Configure the display data for created widget if it has implemented the standard interface.
        	Cast<IPickableInfoWidget>(SubWidget)->Execute_ApplyPickableInfo(SubWidget,
        		Cast<IPickableItem>(ItemAbleToPickup)->Execute_GetPickableInfo(ItemAbleToPickup));
        }
        PickableWidgets.Add(ItemAbleToPickup, SubWidget);
        DisplayBox->AddChildToVerticalBox(SubWidget)->SetPadding(FMargin(0, 0, 0, 20));
		return true;
	}
	return false;
}

bool UPickupFunctionModule::RemoveItemFromList(AActor* ItemLostTrack)
{
	if (!IsValid(DisplayBox) || !IsValid(PickableDisplayLabelClass))
	{
		// To prevent the Display box becomes invalid afterwards.
		return PickableItems.Remove(ItemLostTrack) > 0;
	}
	if (PickableItems.Remove(ItemLostTrack) > 0)
	{
		AController* OwnerController = GetOwner()->GetInstigatorController();
		if (!IsValid(OwnerController) || !OwnerController->IsLocalPlayerController())
		{
			return true;
		}
		DisplayBox->RemoveChild(PickableWidgets[ItemLostTrack]);
        PickableWidgets.Remove(ItemLostTrack);
		return true;
	}
	return false;
}

bool UPickupFunctionModule::PickupItem(AActor* ItemToPickup)
{
	if (PickableItems.Contains(ItemToPickup))
	{
		OnPickupItemEvent.Broadcast(ItemToPickup);
		return true;
	}
	return false;
}

bool UPickupFunctionModule::PickupFirstItem()
{
	UE_LOG(LogTemp, Warning, TEXT("PickupFirstItem"));
	if (PickableItems.Num() == 0) return false;
	UE_LOG(LogTemp, Warning, TEXT("Ahhhhh....."));
	return PickupItem(PickableItems[0]);
}

