// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "Basics/AimableWeapon.h"
#include "Characters/ShooterGameCharacterBase.h"
#include "Weapons/Basics/PickableWeapon.h"
#include "SniperRifle.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class PLATESHOOTINGGAME_API ASniperRifle : public APickableWeapon, public IAimableWeapon
{
	GENERATED_BODY()

	ASniperRifle();

protected:
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category="Settings|Fire")
	float ShootingRange = 10000;
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category="Settings|Fire")
	float ShotImpulse = 1.5;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Components)
	UCameraComponent* Camera;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category="Settings|Fire")
	float NormalDamagePerFire = 200;
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category="Settings|Fire")
	float CriticalDamagePerFire = 400;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category="Settings|Fire")
	TArray<float> ScopeFOV = { 40, 12, 4 };
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category="Status")
	int CurrentScopeFOV = -1;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Settings|Fire")
	USoundCue* FireSoundEffect;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Settings|Fire")
	USoundCue* MagEmptySoundEffect;
	
public:
	virtual void OnFire_Implementation() override;

	virtual void OnEnterScope_Implementation(AShooterGameCharacterBase* Character, AWeapon* Weapon) override;
	virtual void OnAdjustScope_Implementation(AShooterGameCharacterBase* Character, AWeapon* Weapon) override;
	virtual bool CanAdjustNow_Implementation(AShooterGameCharacterBase* Character, AWeapon* Weapon) override;
	virtual void OnExitScope_Implementation(AShooterGameCharacterBase* Character, AWeapon* Weapon) override;

	UFUNCTION(BlueprintImplementableEvent, Category=Process)
	void OnEnterScope_BP(AShooterGameCharacterBase* Character, AWeapon* Weapon);
	UFUNCTION(BlueprintImplementableEvent, Category=Process)
    void OnAdjustScope_BP(AShooterGameCharacterBase* Character, AWeapon* Weapon);
	UFUNCTION(BlueprintImplementableEvent, Category=Process)
    void OnExitScope_BP(AShooterGameCharacterBase* Character, AWeapon* Weapon);

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
