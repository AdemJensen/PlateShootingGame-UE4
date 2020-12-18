// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "Components/SphereComponent.h"
#include "Pickables/PickableItem.h"
#include "Sound/SoundCue.h"
#include "Weapons/Basics/Weapon.h"
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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Components)
	USphereComponent* PickupRange;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Settings)
	USoundCue* PickupSoundEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Settings)
	USoundCue* DropSoundEffect;

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

	// Pickup Events
	
	UFUNCTION(Server, Reliable, BlueprintCallable, Category=Process)
	void HandlePickupActionOnServer(AActor* ActionActor);
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category=Process)
    void SetSimulatePhysicsOnAll(const bool SimulatePhysics);
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category=Process)
    void AnnounceLostTrackOnAll();
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category=Process)
    void PlaySoundOnAll(const bool PickupSound);

	// Drop Events

	UFUNCTION(Server, Reliable, BlueprintCallable, Category=Process)
    void HandleDropActionOnServer();
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category=Process)
    void AnnounceAbleToPickupOnAll();
	
};
