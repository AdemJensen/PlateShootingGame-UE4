// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/PickableWeapon.h"
#include "AssaultRifle.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class PLATESHOOTINGGAME_API AAssaultRifle : public APickableWeapon
{
	GENERATED_BODY()

protected:
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category="Settings|Fire")
	float ShootingRange;
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category="Settings|Fire")
	float ShotImpulse;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category="Settings|Fire")
	float NormalDamagePerFire;
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category="Settings|Fire")
	float CriticalDamagePerFire;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Settings|Fire")
	USoundCue* FireSoundEffect;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Settings|Fire")
	USoundCue* MagEmptySoundEffect;
	
public:
	virtual void OnFire_Implementation() override;

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category=Process)
	virtual void MakeFireEffectOnAll(const bool Fired);

	// You need to implement this in BP.
	// You can make some fire animations play here, or some sound effects play here.
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category=Process)
    void MakeFireEffect_BP(const bool Fired);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category=Process)
    virtual void MakeHitEffectOnAll(UPhysicalMaterial* HitPhysicalMaterial, FVector HitLocation, AActor* HitActor,
    	UPrimitiveComponent* HitComponent, FName HitBoneName);

	// You need to implement this in BP.
	// You can make some hit sound effects or FXs play here.
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category=Process)
	void MakeHitEffect_BP(UPhysicalMaterial* HitPhysicalMaterial, FVector HitLocation, AActor* HitActor,
        UPrimitiveComponent* HitComponent, FName HitBoneName);

	virtual void SetFireDirectionByCameraParameters(const FVector CameraLocation, const FRotator CameraRotation) override;
	
};
