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

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// This TMap actually integrated both TArray[PickableItems] and TMap[Display Items].
	// Now it has both functions.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Status)
	TMap<AActor*, UUserWidget*> PickableWidgetList;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Status)
	UVerticalBox* DisplayBox;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category=Settings)
	TSubclassOf<UUserWidget> PickableDisplayLabelClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Settings)
	FOnPickupItemDelegate OnPickupItemEvent;

public:	
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
