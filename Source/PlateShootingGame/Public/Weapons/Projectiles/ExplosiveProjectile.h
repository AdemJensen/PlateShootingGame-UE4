// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Weapons/Projectiles/Projectile.h"
#include "ExplosiveProjectile.generated.h"

/**
 * 
 */
UCLASS()
class PLATESHOOTINGGAME_API AExplosiveProjectile : public AProjectile
{
	GENERATED_BODY()

public:
	AExplosiveProjectile();

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Status)
	bool bExploding;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category=Settings)
	bool bDestructible;
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category=Settings)
	float Health;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category=Settings)
	float DamageRange;
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category=Settings)
	float ImpulseRange;
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category=Settings)
	float MaxImpulse;
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category=Settings)
	float MaxDamage;
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category=Settings)
	float MinDamage;

	UFUNCTION(Server, Reliable, BlueprintCallable, Category=Actions)
	virtual void Explode_OnServer();
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category=Actions)
    virtual void Explode_OnAll();
	UFUNCTION(BlueprintImplementableEvent, Category=Actions)
    void Explode_BP_OnServer();
	UFUNCTION(BlueprintImplementableEvent, Category=Actions)
    void Explode_BP_OnAll();

	// Called when takes damage and health drops to 0.
	UFUNCTION(Server, Reliable, BlueprintCallable, Category=Actions)
    virtual void OnDestroyed_OnServer();
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category=Actions)
    virtual void OnDestroyed_OnAll();
	UFUNCTION(BlueprintImplementableEvent, Category=Actions)
    void OnDestroyed_BP_OnServer();
	UFUNCTION(BlueprintImplementableEvent, Category=Actions)
    void OnDestroyed_BP_OnAll();

	UPROPERTY()
	TSet<AActor*> ActorsExcludedFromImpulseCalculation;
	// When calculating radius impulse, we will make a sphere scan and get all the overlapped components.
	// Most of the time we do not need this, but sometimes we do not want an Actor being applied impulse multiple times
	// (such as a living Character, we don't want to apply too much impulse to a CharacterMovement Object)
	// In order to exclude an already calculated actor, you can use this function.
	// The Exclude is only valid via one impulse check round, so don't worry.
	UFUNCTION(BlueprintCallable, Category=Tools)
	void ExcludeActorFromCurrentImpulseCalculationRound(AActor* ActorToBan);
	
	// Called from Server, by Explode();
	UFUNCTION(BlueprintCallable, Category=Actions)
    virtual void OnApplyImpulse(AActor* ActorToApplyImpulse, UPrimitiveComponent* ComponentToApplyImpulse, float Distance);
	UFUNCTION(BlueprintImplementableEvent, Category=Actions)
    void OnApplyImpulse_BP(AActor* ActorToApplyImpulse, float Distance);

	// Default implementation is: Explodes when hit.
	// Should be called from SERVER, but cannot guarantee.
	UFUNCTION(BlueprintCallable, Category=Actions)
    virtual void OnHit_OnServer(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category=Actions)
    virtual void OnHit_OnAll(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION(BlueprintImplementableEvent, Category=Actions)
    void OnHit_BP_OnServer(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION(BlueprintImplementableEvent, Category=Actions)
    void OnHit_BP_OnAll(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// Damages
	// Should only be executed via SERVER (Because the Controller will be invalid on Clients)
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION(BlueprintImplementableEvent, Category=Actions)
    void TakeDamage_BP(float DamageAmount, APawn* EventInstigator, AActor* DamageCauser);

	// Tools
	UFUNCTION(BlueprintCallable, Category=Tools)
	void ApplyImpulseOnComponent(UPrimitiveComponent* ComponentToApplyImpulse);
	// Will judge whether the character is simulating Physics, will apply 2 different methods.
	UFUNCTION(BlueprintCallable, Category=Tools)
    void ApplyImpulseOnCharacter(ACharacter* CharacterToApplyImpulse);
	
	// This will calculate the impulse for standing character, who doesn't have SimulatingPhysics opened,
	// but still wants to have a little bit blasted away.
	// Hint: ApplyImpulseOnCharacter has used this function already.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category=Tools)
    FVector CalculateExplosionImpulseForLiveCharacter(const FVector CharacterLocation,
    	const FVector ExplodeLocation, const float ImpulseStrengthFactor);
	
};
