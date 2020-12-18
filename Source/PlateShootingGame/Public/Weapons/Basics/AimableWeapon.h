// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PickableItemDisplayData.h"
#include "Weapon.h"
#include "Characters/ShooterGameCharacterBase.h"


#include "AimableWeapon.generated.h"


UINTERFACE(Blueprintable)
class UAimableWeapon : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IAimableWeapon
{
	GENERATED_IINTERFACE_BODY()

	// Called by Weapon when the owner enters the scope mode.
	// Must make it called from SERVER if you want multiplayer functions.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category=Actions)
	void OnEnterScope(AShooterGameCharacterBase* Character, AWeapon* Weapon);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category=Actions)
    void OnAdjustScope(AShooterGameCharacterBase* Character, AWeapon* Weapon);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category=Actions)
    bool CanAdjustNow(AShooterGameCharacterBase* Character, AWeapon* Weapon);

	// Called by Weapon when the owner exits the scope mode.
	// Must make it called from SERVER if you want multiplayer functions.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category=Actions)
    void OnExitScope(AShooterGameCharacterBase* Character, AWeapon* Weapon);

	// Called by Weapon when the owner exits the scope mode.
	// Must make it called from SERVER if you want multiplayer functions.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category=Actions)
    void SetFireDirectionByAimParameters(AShooterGameCharacterBase* Character, AWeapon* Weapon);
	
};
