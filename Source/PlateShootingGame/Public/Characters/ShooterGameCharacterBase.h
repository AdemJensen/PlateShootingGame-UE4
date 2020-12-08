// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "Weapons/WeaponType.h"
#include "PickupFunctionModule.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Pickables/HoldableActor.h"
#include "Weapons/Weapon.h"

#include "ShooterGameCharacterBase.generated.h"

UCLASS()
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
	float MaxHealth;

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

	// Actions On Server:

	UFUNCTION(Server, Reliable, BlueprintCallable, Category=Process)
    void HandleAction_Fire_StartOnServer();
	UFUNCTION(Server, Reliable, BlueprintCallable, Category=Process)
    void HandleAction_Fire_StopOnServer();
	UFUNCTION(Server, Reliable, BlueprintCallable, Category=Process)
    void HandleAction_PickupOnServer();
	
};
