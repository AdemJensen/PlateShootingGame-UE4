// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "Weapons/Basics/WeaponType.h"
#include "PickupFunctionModule.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Pickables/HoldableActor.h"
#include "Weapons/Basics/Weapon.h"

#include "ShooterGameCharacterBase.generated.h"

UCLASS(Blueprintable, BlueprintType)
class PLATESHOOTINGGAME_API AShooterGameCharacterBase : public ACharacter, public IHoldableActor
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterGameCharacterBase();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Components)
	UPickupFunctionModule* PickupFunctionModule;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Components)
	USpringArmComponent* SpringArm;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Components)
	UCameraComponent* Camera;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category=Settings)
	float MaxHealth = 100;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Settings)
	float MovementInputFactor = 1.0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Settings)
	float ViewInputFactor = 1.0;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category="Status|View")
	float PitchAngle;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category="Status|View")
	float YawAngle;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category="Status")
	float Health;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category="Status")
	bool bDead;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category="Status")
	bool bAiming;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Status")
	bool bFreeView;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category="Status")
	AWeapon* WeaponInHand;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category=Status)
	bool IsAttacking();
	// This function will be called when there is no weapon in hand.
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category=Status)
    bool IsAttackingWithNoWeaponInHand_BP();

	UFUNCTION(BlueprintCallable, Category=Status)
    float GetHealthPercentage() const { return Health / MaxHealth; }

	UFUNCTION(BlueprintCallable, Category=Status)
    float GetHealthFromPercentage(const float Percentage) const { return MaxHealth * Percentage; }

	// Add health. Will do nothing and return 0 if dead.
	UFUNCTION(BlueprintCallable, Category=Status)
    float AddHealth(const float HealthToAdd);

	UFUNCTION(BlueprintPure, BlueprintCallable, Category=Status)
	EWeaponType GetWeaponType();

	// Pickup Functions:
	
	virtual void OnAbleToPickupItem_Implementation(AActor* ItemAbleToPickup) override;
	virtual void OnLostTrackOfItem_Implementation(AActor* ItemLostTrackOf) override;

	// The input must be guaranteed to be a IPickableItem implemented object.
	UFUNCTION(Server, Reliable, BlueprintCallable, Category=Process)
	void HandlePickupOnServer(AActor* ItemToPickup);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category=Process)
    void HandleDropOnServer();

	// To attach the Weapon in hand or detach it from hand, depends on the boolean parameter.
	UFUNCTION(Server, Reliable, BlueprintCallable, Category=Process)
    void ChangeWeaponInHandOnAll(const bool Attach);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category=Process)
    void CallItemInterfaceFunctionOnAll(const bool Pickup, AActor* ItemForAction);


	// Input Actions:

	UFUNCTION(BlueprintCallable, Category=Inputs)
	virtual void InputAxis_MoveForward(const float AxisValue);
	UFUNCTION(BlueprintCallable, Category=Inputs)
    virtual void InputAxis_MoveRight(const float AxisValue);
	UFUNCTION(BlueprintCallable, Category=Inputs)
    virtual void InputAxis_LookUp(const float AxisValue);
	UFUNCTION(BlueprintCallable, Category=Inputs)
    virtual void InputAxis_TurnRight(const float AxisValue);

	UFUNCTION(BlueprintCallable, Category=Inputs)
    virtual void InputAction_Jump_Start();
	UFUNCTION(BlueprintCallable, Category=Inputs)
    virtual void InputAction_Jump_Stop();
	UFUNCTION(BlueprintCallable, Category=Inputs)
    virtual void InputAction_Fire_Start();
	UFUNCTION(BlueprintCallable, Category=Inputs)
    virtual void InputAction_Fire_Stop();
	UFUNCTION(BlueprintCallable, Category=Inputs)
    virtual void InputAction_Pickup();
	UFUNCTION(BlueprintCallable, Category=Inputs)
    virtual void InputAction_Drop();
	UFUNCTION(BlueprintCallable, Category=Inputs)
    virtual void InputAction_Aim();
	UFUNCTION(Client, Reliable, BlueprintCallable, Category=Inputs)
	virtual void InputAction_Aim_Start();
	UFUNCTION(Client, Reliable, BlueprintCallable, Category=Inputs)
    virtual void InputAction_Aim_Stop();

	// Actions On Server:

	UFUNCTION(Server, Reliable, BlueprintCallable, Category=Process)
    void HandleAction_Fire_StartOnServer();
	UFUNCTION(Server, Reliable, BlueprintCallable, Category=Process)
    void HandleAction_Fire_StopOnServer();
	UFUNCTION(Server, Reliable, BlueprintCallable, Category=Process)
    void HandleAction_PickupOnServer();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category=Process)
    void HandleAction_Aim_StartOnServer();
	UFUNCTION(Server, Reliable, BlueprintCallable, Category=Process)
    void HandleAction_Aim_StopOnServer();
	UFUNCTION(BlueprintImplementableEvent, Category=Process)
    void HandleAction_Aim_Start_BP();
	UFUNCTION(BlueprintImplementableEvent, Category=Process)
    void HandleAction_Aim_Stop_BP();
	
    
    // Damage and death:

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION(BlueprintImplementableEvent, Category=Actions)
    void TakeDamage_BP(float DamageAmount, APawn* EventInstigator, AActor* DamageCauser);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category=Actions)
	virtual void ActionDie(APawn* EventInstigator, AActor* DamageCauser);
	UFUNCTION(BlueprintImplementableEvent, Category=Actions)
    void ActionDie_BP_OnAll(APawn* EventInstigator, AActor* DamageCauser);
	UFUNCTION(BlueprintImplementableEvent, Category=Actions)
    void ActionDie_BP_OnServer(APawn* EventInstigator, AActor* DamageCauser);

	UFUNCTION(Client, Reliable, BlueprintCallable, Category=Actions)
    virtual void OnConfirmedKill(APawn* KilledPawn);
	UFUNCTION(BlueprintImplementableEvent, Category=Actions)
    void OnConfirmedKill_BP_OnClient(APawn* KilledPawn);

	UFUNCTION(Client, Reliable, BlueprintCallable, Category=Actions)
    virtual void OnConfirmedHit(AActor* HitActor);
	UFUNCTION(BlueprintImplementableEvent, Category=Actions)
    void OnConfirmedHit_BP_OnClient(AActor* HitActor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category=Calculations)
	void AdjustShootingDirection();
	
};
