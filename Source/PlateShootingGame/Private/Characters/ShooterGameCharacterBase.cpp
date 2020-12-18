// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ShooterGameCharacterBase.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GeneratedCodeHelpers.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Pickables/PickableItem.h"
#include "Weapons/Basics/AimableWeapon.h"
#include "Weapons/Basics/PickableWeapon.h"

// Sets default values
AShooterGameCharacterBase::AShooterGameCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	PickupFunctionModule = CreateDefaultSubobject<UPickupFunctionModule>("PickupFunctionModule");
	PickupFunctionModule->OnPickupItemEvent.AddDynamic(this, &AShooterGameCharacterBase::HandlePickupOnServer);
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);

	SpringArm->bUsePawnControlRotation = true;

	// GetMesh()->SetMassOverrideInKg(NAME_None, 75.0);
	GetMesh()->BodyInstance.bOverrideMass = true;
	GetMesh()->BodyInstance.SetMassOverride(0.f);
	// This has been replaced due to GEngine fault.
	// For more info, see https://answers.unrealengine.com/questions/419964/gengine-not-initialized-problem.html
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionObjectType(ECC_Pawn);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Block);
}

void AShooterGameCharacterBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AShooterGameCharacterBase, MaxHealth);
	DOREPLIFETIME(AShooterGameCharacterBase, PitchAngle);
	DOREPLIFETIME(AShooterGameCharacterBase, YawAngle);
	DOREPLIFETIME(AShooterGameCharacterBase, Health);
	DOREPLIFETIME(AShooterGameCharacterBase, bDead);
	DOREPLIFETIME(AShooterGameCharacterBase, bAiming);
	DOREPLIFETIME(AShooterGameCharacterBase, WeaponInHand);
}

// Called when the game starts or when spawned
void AShooterGameCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();

	Health = MaxHealth;
	
}

// Called every frame
void AShooterGameCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsLocallyControlled() || GetLocalRole() == ROLE_Authority)
	{
		FMinimalViewInfo DesiredView;
		Camera->GetCameraView(0, DesiredView);
		const FRotator CameraRotation = DesiredView.Rotation - GetActorRotation();
		YawAngle = CameraRotation.Yaw;	// This has been modified, the original value is [Roll]!
		PitchAngle = CameraRotation.Pitch;

	}

	UWorld* World = GetWorld();
	if (IsValid(World) && World->IsServer())
	{
		// Update weapon aim direction.
		if (IsValid(WeaponInHand))
		{
			// UE_LOG(LogTemp, Error, TEXT("%s %s"), *GetName(), GIsServer ? TEXT("true") : TEXT("false"));
			AdjustShootingDirection();
		}
	}
}

// Called to bind functionality to input
void AShooterGameCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Axis
	PlayerInputComponent->BindAxis("MoveForward", this, &AShooterGameCharacterBase::InputAxis_MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShooterGameCharacterBase::InputAxis_MoveRight);
	PlayerInputComponent->BindAxis("TurnRate", this, &AShooterGameCharacterBase::InputAxis_TurnRight);	// Right = +
	PlayerInputComponent->BindAxis("Turn", this, &AShooterGameCharacterBase::InputAxis_TurnRight);		// Right = +
	PlayerInputComponent->BindAxis("LookUpRate", this, &AShooterGameCharacterBase::InputAxis_LookUp);
	PlayerInputComponent->BindAxis("LookUp", this, &AShooterGameCharacterBase::InputAxis_LookUp);

	// Keys
	// PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AShooterGameCharacterBase::HandleDropOnServer);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AShooterGameCharacterBase::InputAction_Fire_Start);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AShooterGameCharacterBase::InputAction_Fire_Stop);
	// PlayerInputComponent->BindAction("OpenInGameMenu", IE_Pressed, this, &AShooterGameCharacterBase::HandleDropOnServer);
	// PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AShooterGameCharacterBase::HandleDropOnServer);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AShooterGameCharacterBase::InputAction_Jump_Start);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AShooterGameCharacterBase::InputAction_Jump_Stop);
	// PlayerInputComponent->BindAction("FreeView", IE_Pressed, this, &AShooterGameCharacterBase::HandleDropOnServer);
	PlayerInputComponent->BindAction("PickUp", IE_Pressed, this, &AShooterGameCharacterBase::InputAction_Pickup);
	PlayerInputComponent->BindAction("DropWeapon", IE_Pressed, this, &AShooterGameCharacterBase::InputAction_Drop);
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AShooterGameCharacterBase::InputAction_Aim);

}

bool AShooterGameCharacterBase::IsAttacking()
{
	if (IsValid(WeaponInHand))
	{
		return WeaponInHand->IsFiring();
	}
	return IsAttackingWithNoWeaponInHand_BP();
}

float AShooterGameCharacterBase::AddHealth(const float HealthToAdd)
{
	if (bDead)
	{
		return 0;
	}
	if (MaxHealth - Health > HealthToAdd)
	{
		Health += HealthToAdd;
		return HealthToAdd;
	}
	const float AddedHealth = MaxHealth - Health;
	Health = MaxHealth;
	return AddedHealth;
}

EWeaponType AShooterGameCharacterBase::GetWeaponType()
{
	return IsValid(WeaponInHand) ? WeaponInHand->WeaponType : EWeaponType::EMPTY;
}

void AShooterGameCharacterBase::OnAbleToPickupItem_Implementation(AActor* ItemAbleToPickup)
{
	PickupFunctionModule->AddItemToList(ItemAbleToPickup);
}

void AShooterGameCharacterBase::OnLostTrackOfItem_Implementation(AActor* ItemLostTrackOf)
{
	PickupFunctionModule->RemoveItemFromList(ItemLostTrackOf);
}

void AShooterGameCharacterBase::InputAxis_MoveForward(const float AxisValue)
{
	AddMovementInput(UKismetMathLibrary::GetForwardVector(
		FRotator(0, GetControlRotation().Yaw, 0)
		), AxisValue * MovementInputFactor);
}

void AShooterGameCharacterBase::InputAxis_MoveRight(const float AxisValue)
{
	AddMovementInput(UKismetMathLibrary::GetRightVector(
        FRotator(0, GetControlRotation().Yaw, 0)
        ), AxisValue * MovementInputFactor);
}

void AShooterGameCharacterBase::InputAxis_LookUp(const float AxisValue)
{
	AddControllerPitchInput(AxisValue * ViewInputFactor);
}

void AShooterGameCharacterBase::InputAxis_TurnRight(const float AxisValue)
{
	AddControllerYawInput(AxisValue * ViewInputFactor);
}

void AShooterGameCharacterBase::InputAction_Jump_Start()
{
	Jump();
}

void AShooterGameCharacterBase::InputAction_Jump_Stop()
{
	StopJumping();
}

void AShooterGameCharacterBase::InputAction_Fire_Start()
{
	HandleAction_Fire_StartOnServer();
}

void AShooterGameCharacterBase::InputAction_Fire_Stop()
{
	HandleAction_Fire_StopOnServer();
}

void AShooterGameCharacterBase::InputAction_Pickup()
{
	//UE_LOG(LogTemp, Warning, TEXT("InputAction_Pickup"));
	HandleAction_PickupOnServer();
}

void AShooterGameCharacterBase::InputAction_Drop()
{
	if (IsValid(WeaponInHand))
	{
		InputAction_Aim_Stop();
	}
	HandleDropOnServer();
}

void AShooterGameCharacterBase::InputAction_Aim()
{
	if (IsValid(WeaponInHand) && WeaponInHand->GetClass()->ImplementsInterface(UAimableWeapon::StaticClass()))
	{
		if (bAiming)
        {
			if (Cast<IAimableWeapon>(WeaponInHand)->Execute_CanAdjustNow(WeaponInHand, this, WeaponInHand))
			{
				Cast<IAimableWeapon>(WeaponInHand)->Execute_OnAdjustScope(WeaponInHand, this, WeaponInHand);
			}
			else
			{
				InputAction_Aim_Stop();
			}
        }
        else
        {
        	InputAction_Aim_Start();
        }
	}
	
}

void AShooterGameCharacterBase::InputAction_Aim_Start()
{
	if (IsValid(WeaponInHand) && WeaponInHand->GetClass()->ImplementsInterface(UAimableWeapon::StaticClass()))
    {
		//UE_LOG(LogTemp, Warning, TEXT("Entering Aim"));
		bAiming = true;
    	Cast<IAimableWeapon>(WeaponInHand)->Execute_OnEnterScope(WeaponInHand, this, WeaponInHand);
		HandleAction_Aim_Start_BP();
		HandleAction_Aim_StartOnServer();
    }
}

void AShooterGameCharacterBase::InputAction_Aim_Stop()
{
	if (IsValid(WeaponInHand) && WeaponInHand->GetClass()->ImplementsInterface(UAimableWeapon::StaticClass()))
    {
		//UE_LOG(LogTemp, Warning, TEXT("Exiting Aim"));
		bAiming = false;
    	Cast<IAimableWeapon>(WeaponInHand)->Execute_OnExitScope(WeaponInHand, this, WeaponInHand);
		HandleAction_Aim_Stop_BP();
		HandleAction_Aim_StopOnServer();
    }
}

void AShooterGameCharacterBase::HandleAction_Aim_StartOnServer_Implementation()
{
	bAiming = true;
}

void AShooterGameCharacterBase::HandleAction_Aim_StopOnServer_Implementation()
{
	bAiming = false;
}

float AShooterGameCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
                                            AController* EventInstigator, AActor* DamageCauser)
{
	if (bDead)
	{
		return 0;
	}
	Health -= DamageAmount;
	const auto KillerPawn = IsValid(EventInstigator) ? EventInstigator->GetPawn() : nullptr;
	const auto KillerCharacter = IsValid(EventInstigator) ? EventInstigator->GetCharacter() : nullptr;
	TakeDamage_BP(DamageAmount, KillerPawn, DamageCauser);
	if (Health <= 0)
	{
	 	if (HasAuthority())
	 	{
	 		bDead = true;
	 		ActionDie_BP_OnServer(KillerPawn, DamageCauser);
	 		ActionDie(KillerPawn, DamageCauser);
			if (IsValid(KillerCharacter) && KillerCharacter->GetClass()->IsChildOf(StaticClass()))
			{
				Cast<AShooterGameCharacterBase>(KillerCharacter)->OnConfirmedKill(this);
			}
	 	}
	}
	else
	{
		if (HasAuthority())
		{
	 		if (IsValid(KillerCharacter) && KillerCharacter->GetClass()->IsChildOf(StaticClass()))
	 		{
	 			Cast<AShooterGameCharacterBase>(KillerCharacter)->OnConfirmedHit(this);
	 		}
	 	}
	}
	return DamageAmount;
}

void AShooterGameCharacterBase::AdjustShootingDirection_Implementation()
{
	if (WeaponInHand->GetClass()->ImplementsInterface(UAimableWeapon::StaticClass()) && bAiming)
	{
		Cast<IAimableWeapon>(WeaponInHand)->Execute_SetFireDirectionByAimParameters(WeaponInHand, this, WeaponInHand);
	}
	else
	{
		WeaponInHand->SetFireDirectionByCameraParameters(Camera->GetComponentLocation(), Camera->GetComponentRotation());
	}
	if (!bAiming)
	{
		WeaponInHand->ApplyFireDirectionRandomOffset(2);
	}
}

void AShooterGameCharacterBase::OnConfirmedHit_Implementation(AActor* HitActor)
{
	OnConfirmedHit_BP_OnClient(HitActor);
}

void AShooterGameCharacterBase::OnConfirmedKill_Implementation(APawn* KilledPlayer)
{
	OnConfirmedKill_BP_OnClient(KilledPlayer);
}

void AShooterGameCharacterBase::ActionDie_Implementation(APawn* EventInstigator, AActor* DamageCauser)
{
	// GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Destructible, ECR_Block);
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetAllBodiesPhysicsBlendWeight(1.0);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	if (IsValid(WeaponInHand))
	{
		if (WeaponInHand->GetClass()->IsChildOf(APickableWeapon::StaticClass()))
		{
			InputAction_Drop();
			//HandleDropOnServer();
		}
	}
	bDead = true;
	AController* InstigatorController = GetController();
	if (IsValid(InstigatorController) && InstigatorController->GetClass()->IsChildOf(APlayerController::StaticClass()))
	{
		DisableInput(Cast<APlayerController>(InstigatorController));
	}
	ActionDie_BP_OnAll(EventInstigator, DamageCauser);
}

void AShooterGameCharacterBase::HandleAction_PickupOnServer_Implementation()
{
	//UE_LOG(LogTemp, Warning, TEXT("HandleAction_PickupOnServer_Implementation"));
	PickupFunctionModule->PickupFirstItem();
}

void AShooterGameCharacterBase::HandleAction_Fire_StopOnServer_Implementation()
{
	if (IsValid(WeaponInHand))
	{
		WeaponInHand->ActionStopFire();
	}
}

void AShooterGameCharacterBase::HandleAction_Fire_StartOnServer_Implementation()
{
	if (IsValid(WeaponInHand))
	{
		WeaponInHand->ActionFire();
	}
}

void AShooterGameCharacterBase::CallItemInterfaceFunctionOnAll_Implementation(const bool Pickup, AActor* ItemForAction)
{
	IPickableItem* ActionItem = Cast<IPickableItem>(ItemForAction);
	if (Pickup)
	{
		if (!IsValid(ItemForAction))
		{
			UE_LOG(LogTemp, Warning, TEXT("Error when executing CallItemInterfaceFunctionOnAll_Implementation: ItemForAction is Null, the env is %s"), GetWorld()->IsServer() ? TEXT("Server") : TEXT("Client"));
			return;
		}
		ActionItem->Execute_OnPickup(ItemForAction, this);
	}
	else
	{
		ActionItem->Execute_OnDrop(ItemForAction);
	}
}

void AShooterGameCharacterBase::ChangeWeaponInHandOnAll_Implementation(const bool Attach)
{
	if (Attach)
	{
		WeaponInHand->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true),
			FName("Weapon_Rifle_Hand_R"));
	}
	else
	{
		WeaponInHand->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld,
			EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, true));
	}
}

void AShooterGameCharacterBase::HandleDropOnServer_Implementation()
{
	if (IsValid(WeaponInHand))
	{
		ChangeWeaponInHandOnAll(false);
		CallItemInterfaceFunctionOnAll(false, WeaponInHand);
		WeaponInHand = nullptr;
	}
}

// Be aware that this should be called from PickupFunctionModule!
void AShooterGameCharacterBase::HandlePickupOnServer_Implementation(AActor* ItemToPickup)
{
	if (ItemToPickup->GetClass()->IsChildOf(APickableWeapon::StaticClass()))
	{
		AWeapon* WeaponToPickup = Cast<AWeapon>(ItemToPickup);
		if (IsValid(WeaponInHand))
		{
			InputAction_Drop();
			//HandleDropOnServer();		// No worries to the execution sequence because they are both on server, which
										// doesn't cause network replication and delay.
		}
		WeaponInHand = WeaponToPickup;
		ChangeWeaponInHandOnAll(true);	// Client Attach weapon.
	}
	CallItemInterfaceFunctionOnAll(true, ItemToPickup);
}

