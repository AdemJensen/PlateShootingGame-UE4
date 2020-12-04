// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "Components/SphereComponent.h"
#include "Pickables/PickableItem.h"
#include "Weapons/Weapon.h"
#include "PickableWeapon.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class PLATESHOOTINGGAME_API APickableWeapon : public AWeapon, public IPickableItem
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	APickableWeapon();

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Components)
	USphereComponent* PickupRange;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Status)
	TArray<AActor*> ActorsWaitingToPickMeUp;

public:

	virtual void OnPickup_Implementation(AActor* ActionActor) override;
	virtual void OnDrop_Implementation() override;
	virtual FPickableItemDisplayData GetPickableInfo_Implementation() override;
	virtual bool IsOpenForPickup_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category=Actions)
	void OnPickupRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category=Actions)
	void OnPickupRangeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
};
