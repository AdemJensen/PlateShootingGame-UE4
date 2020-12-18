// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "Characters/ShooterGameCharacterBase.h"
#include "Weapons/Basics/PickableWeapon.h"
#include "Weapons/Projectiles/Projectile.h"

#include "ProjectileSpawnerWeapon.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class PLATESHOOTINGGAME_API AProjectileSpawnerWeapon : public APickableWeapon
{
	GENERATED_BODY()

public:
	AProjectileSpawnerWeapon();

protected:
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category="Settings|Fire")
	float InitialProjectileVelocity = 1500;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Settings|Fire")
	USoundCue* FireSoundEffect;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Settings|Fire")
	USoundCue* MagEmptySoundEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Settings|Fire")
	TSubclassOf<AProjectile> ProjectileClass;
	
public:
	virtual void OnFire_Implementation() override;

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category=Process)
	virtual void MakeFireEffectOnAll(const bool Fired);

	// You need to implement this in BP.
	// You can make some fire animations play here, or some sound effects play here.
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category=Process)
    void MakeFireEffect_BP(const bool Fired);

	// This function runs on server(Driven by OnFire())
	UFUNCTION(BlueprintCallable, Category=Process)
    virtual void ConfigureProjectile(AProjectile* Projectile);
	// This function runs on server(Driven by OnFire())
	UFUNCTION(BlueprintImplementableEvent, Category=Process)
    void ConfigureProjectile_BP(AProjectile* Projectile);

	virtual void SetFireDirectionByCameraParameters(const FVector CameraLocation, const FRotator CameraRotation) override;
	
};
