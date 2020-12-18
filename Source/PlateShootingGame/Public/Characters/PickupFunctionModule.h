// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/VerticalBox.h"

#include "PickupFunctionModule.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickupItemDelegate, AActor*, ItemToPickup);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLATESHOOTINGGAME_API UPickupFunctionModule : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPickupFunctionModule();

	// Called when the game starts
	virtual void BeginPlay() override;

	// This TMap actually integrated both TArray[PickableItems] and TMap[Display Items].
	// Now it has both functions.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Status)
	TMap<AActor*, UUserWidget*> PickableWidgets;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Status)
	TArray<AActor*> PickableItems;

	// Sometimes, when a player loads in a level with a weapon on his side,
	// the system will execute the Overlap event first, causing the list to be empty.
	// We will scan all the objects surrounds, so that no object will be missed.
	// This array should be useless after game start.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Status)
	bool bShouldScanPickable = true;

	// For AI controlled characters.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Settings)
	bool bUsePickableWidget = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Status)
	UVerticalBox* DisplayBox;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category=Settings)
	TSubclassOf<UUserWidget> PickableDisplayLabelClass;

	bool bDisplayLabelClassProperlySetup = false;
	
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Settings)
	FOnPickupItemDelegate OnPickupItemEvent;
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category=Actions)
	virtual bool AddItemToList(AActor* ItemAbleToPickup);

	UFUNCTION(BlueprintCallable, Category=Actions)
    virtual bool RemoveItemFromList(AActor* ItemLostTrack);

	UFUNCTION(BlueprintCallable, Category=Actions)
    virtual bool PickupItem(AActor* ItemToPickup);

	UFUNCTION(BlueprintCallable, Category=Actions)
    virtual bool PickupFirstItem();
		
};
