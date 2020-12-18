// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Characters/ShooterGameCharacterBase.h"
#include "Projectile.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class PLATESHOOTINGGAME_API AProjectile : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Components)
	UArrowComponent* ProjectileArrow;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Components)
	UStaticMeshComponent* ProjectileBody;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Components)
	UCapsuleComponent* CollisionBox;
	
public:
	AProjectile();

	// This only takes effect when the weapon is pickable or Enables Physics Simulation.
	// Though, we are adding this for universal uses.
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category="Settings")
	float MassInKg;
	
};
