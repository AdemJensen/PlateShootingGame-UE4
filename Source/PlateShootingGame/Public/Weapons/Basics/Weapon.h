// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "WeaponType.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReloadInterruptSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMagEmptySignature, bool, bFiredLasstTime);
UCLASS(Blueprintable, BlueprintType)
class PLATESHOOTINGGAME_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Components)
	UArrowComponent* WeaponFireArrow;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Components)
	USkeletalMeshComponent* WeaponBody;

public:
	
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category=Settings)
	EWeaponType WeaponType;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category=Status)
	bool bFiring = false;
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category=Status)
	bool bReloading = false;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category=Status)
	float MagRemain;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Status)
	float TimeSinceLastFire;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category=Settings)
	AActor* WeaponOwner;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category="Settings|Fire")
	float FireInterval;
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category="Settings|Fire")
	bool bFireContinuously;
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category="Settings|Fire")
	bool bNeedsReload;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category="Settings|Fire")
	float MagCapacity;

	// The fire direction of this weapon.
	// This direction is not used or changed by default. You can use it yourself.
	// We DO NOT replicate this because you need to handle all the fire judgements on server.
	// If you need something such as a FX, please create a new function in the sub class.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Settings|Fire")
	FVector FireDirection;

	// This damage rate is just a universal property opened outside.
	// It is recommended that you use this Damage Rate, rather than modifying it within the Child Classes.
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category="Settings|Fire")
	float DamageRate = 1.0;
	
	// This only takes effect when the weapon is pickable or Enables Physics Simulation.
	// Though, we are adding this for universal uses.
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category="Settings")
	float MassInKg;

protected:
	UPROPERTY(BlueprintAssignable, Category=ActionBindings)
	FReloadInterruptSignature OnReloadInterrupted;

	UPROPERTY(BlueprintAssignable, Category=ActionBindings)
	FMagEmptySignature OnMagEmpty;

	
	// A preset bullet direction calculation function.
	// This function calculates the direction by a line trace from camera, and set it to the FireDirection.
	// If no trace result, the FireDirection will remain as-is.
	// @return true if trace successful, and false if not traced anything.
	UFUNCTION(BlueprintCallable, Category="Functions|Fire|Prefebs")
    bool CalculateFireDirectionByLineTrace(const FVector CameraLocation, const FRotator CameraRotation, float TraceRange);

	// A preset bullet direction calculation function.
	// This function calculates the direction by using a equation, and set it to the FireDirection.
	// The EQUATION is: CameraLocation + CameraDirection * 1000f - GunPortLocation
	UFUNCTION(BlueprintCallable, Category="Functions|Fire|Prefebs")
    void CalculateFireDirectionByAdjustmentAlgorithm(const FVector CameraLocation, const FRotator CameraRotation);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// General functions
	
	UFUNCTION(BlueprintCallable, Category="Functions|Fire")
	virtual FVector GetGunPortLocation();
	UFUNCTION(BlueprintCallable, Category="Functions|Fire")
    virtual FRotator GetGunPortRotation();
	UFUNCTION(BlueprintCallable, Category="Functions|Fire")
    virtual FVector GetGunPortDirection();

	// This sets the AimingDirection variable from Rotation (World).
	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Functions|Fire")
    virtual void SetFireRotation(const FRotator AimingRotation);

	// This sets the AimingDirection variable from Directional Unit Vector (World).
	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Functions|Fire")
    virtual void SetFireDirection(const FVector Direction);

	UFUNCTION(BlueprintCallable, Category="Functions|Fire")
    virtual float UseAmmo(const float ExpectedAmmo, bool bUsePartial);

	// Set the fire bullet direction by these camera parameters.
	// Must be implemented, and must be called in the Character BP in TICK().
	// Previously defined in Character, but now moved here.
	UFUNCTION(BlueprintCallable, Category="Functions|Fire")
	virtual void SetFireDirectionByCameraParameters(const FVector CameraLocation, const FRotator CameraRotation);

	// You can use that to check how much the adjusted value has effected the fire direction.
	UFUNCTION(BlueprintCallable, Category="Functions|Fire|Prefebs")
	float GetAdjustedFireBankAngle();

	UFUNCTION(BlueprintCallable, Category=Process)
    void ApplyFireDirectionRandomOffset(const float PrecisionRange);

	// The following functions are considered "Events", which should not be called directly from other BPs or Classes.

	// Remember to call the ancestor's OnReloadStart()
	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Events|Reload")
	virtual void OnReloadStart();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Events|Reload")
    void OnReloadStart_BP();

	// Remember to call the ancestor's OnReloadComplete()
	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Events|Reload")
    virtual void OnReloadComplete();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Events|Reload")
    void OnReloadComplete_BP();

	// Remember to call the ancestor's OnReloadInterrupt()
	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Events|Reload")
    virtual void OnReloadInterrupt();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Events|Reload")
    void OnReloadInterrupt_BP();

	// Remember to call the ancestor's OnFire()
	// Note that this is not the [fire] you would presume. This is invoked via TICK() or ACTION_FIRE()
	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Events|Fire")
    virtual void OnFire();
	// Note that this is not the [fire] you would presume. This is invoked via TICK() or ACTION_FIRE()
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Events|Fire")
    void OnFire_BP();

	// The following functions are considered "Actions", which should be called directly from other BPs or Classes.

	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Actions|Fire")
	virtual void ActionFire();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Actions|Fire")
    virtual void ActionStopFire();
	
	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Actions|Reload")
    virtual void ActionReload();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Actions|Reload")
    virtual void ActionStopReload(const bool GenerateInterruption);
    
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Status|Fire")
    bool IsFiring();
	
};
