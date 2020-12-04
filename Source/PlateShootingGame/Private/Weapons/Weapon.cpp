// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Weapon.h"

#include "GeneratedCodeHelpers.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	WeaponBody = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponBody");
	WeaponFireArrow = CreateDefaultSubobject<UArrowComponent>("WeaponDirection");

	RootComponent = WeaponBody;
	WeaponFireArrow->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Fire
	if (GIsServer)
	{
		if (TimeSinceLastFire < FireInterval * 2)
		{
			TimeSinceLastFire += DeltaTime;
		}
		if (bFiring && TimeSinceLastFire > FireInterval)
		{
			TimeSinceLastFire = FMath::Fmod(TimeSinceLastFire, FireInterval);
			OnFire();
			if (!bFireContinuously)
			{
				ActionStopFire();
			}
		}
	}
	
}

void AWeapon::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeapon, bFiring);
	DOREPLIFETIME(AWeapon, bReloading);
	DOREPLIFETIME(AWeapon, MagRemain);
	DOREPLIFETIME(AWeapon, WeaponOwner);
	DOREPLIFETIME(AWeapon, FireInterval);
	DOREPLIFETIME(AWeapon, bFireContinuously);
	DOREPLIFETIME(AWeapon, bNeedsReload);
	DOREPLIFETIME(AWeapon, MagCapacity);
	DOREPLIFETIME(AWeapon, DamageRate);
	DOREPLIFETIME(AWeapon, MassInKg);
}

FVector AWeapon::GetWeaponLocation()
{
	return WeaponFireArrow->GetComponentLocation();
}

FRotator AWeapon::GetWeaponRotation()
{
	return WeaponFireArrow->GetComponentRotation();
}

FVector AWeapon::GetWeaponDirection()
{
	return WeaponFireArrow->GetComponentRotation().RotateVector(FVector::ForwardVector);
}

void AWeapon::SetFireRotation_Implementation(const FRotator AimingRotation)
{
	FireDirection = AimingRotation.RotateVector(FVector::ForwardVector);
}

void AWeapon::SetFireDirection_Implementation(const FVector Direction)
{
	FireDirection = Direction;
}

float AWeapon::UseAmmo(const float ExpectedAmmo, bool bUsePartial)
{
	if (!bNeedsReload)
	{
		return ExpectedAmmo;
	}
	if (MagRemain < MagCapacity)
	{
		if (bUsePartial)
		{
			const int Remain = MagRemain;
            MagRemain = 0;
            return Remain;
		}
		return 0;	// Cannot use partial, then don't use at all.
	}
	MagRemain -= ExpectedAmmo;
    return ExpectedAmmo;
}

void AWeapon::OnFire_Implementation()
{
	OnFire_BP();
}

void AWeapon::OnReloadInterrupt_Implementation()
{
	OnReloadInterrupt_BP();
}

void AWeapon::OnReloadComplete_Implementation()
{
	OnReloadComplete_BP();
}

void AWeapon::OnReloadStart_Implementation()
{
	OnReloadStart_BP();
}

void AWeapon::ActionFire_Implementation()
{
	UE_LOG(LogTemp, Display, TEXT("AWeapon::ActionFire()"));
	if (!bFiring)
	{
		if (bReloading)
		{
			if (MagRemain > 0)
			{
				ActionStopReload(true);
			}
			else
			{
				return;
			}
		}
		bFiring = true;
	}
}

void AWeapon::ActionStopFire_Implementation()
{
	bFiring = false;
}

void AWeapon::ActionReload_Implementation()
{
	if (bNeedsReload && !bReloading)
	{
		if (MagRemain < MagCapacity)
		{
			bReloading = true;
			if (bFiring) ActionStopFire();
			OnReloadStart();
		}
	}
}

void AWeapon::ActionStopReload_Implementation(const bool GenerateInterruption)
{
	if (bReloading)
	{
		bReloading = false;
		OnReloadInterrupt();
		if (GenerateInterruption)
		{
			OnReloadInterrupted.Broadcast();
		}
	}
}

