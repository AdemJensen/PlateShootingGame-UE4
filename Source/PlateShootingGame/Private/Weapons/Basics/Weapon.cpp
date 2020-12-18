// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Basics/Weapon.h"

#include "GeneratedCodeHelpers.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
    AActor::SetReplicateMovement(true);

	WeaponBody = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponBody");
	WeaponFireArrow = CreateDefaultSubobject<UArrowComponent>("WeaponDirection");

	RootComponent = WeaponBody;
	WeaponFireArrow->SetupAttachment(RootComponent);

	WeaponBody->BodyInstance.bOverrideMass = true;
	WeaponBody->BodyInstance.SetMassOverride(MassInKg);

	MagRemain = MagCapacity;
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
	UWorld* World = GetWorld();
	if (IsValid(World) && World->IsServer())
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
	DOREPLIFETIME(AWeapon, WeaponType);
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

FRotator AWeapon::GetGunPortRotation()
{
	return WeaponFireArrow->GetComponentRotation();
}

FVector AWeapon::GetGunPortDirection()
{
	return WeaponFireArrow->GetComponentRotation().RotateVector(FVector::ForwardVector);
}

FVector AWeapon::GetGunPortLocation()
{
	return WeaponFireArrow->GetComponentLocation();
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
	if (MagRemain < ExpectedAmmo)
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

// This is the basic implementation of all the weapon fire directions.
// This implementation usually won't work well in TPS games.
void AWeapon::SetFireDirectionByCameraParameters(const FVector CameraLocation, const FRotator CameraRotation)
{
	FireDirection = GetGunPortDirection();
}

bool AWeapon::CalculateFireDirectionByLineTrace(const FVector CameraLocation, const FRotator CameraRotation, float TraceRange)
{
	const FVector CameraDirection = CameraRotation.RotateVector(FVector::ForwardVector).GetSafeNormal();
	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		FHitResult HitResult;
		if (World->LineTraceSingleByChannel(HitResult, CameraLocation + CameraDirection,
            CameraLocation + CameraDirection * TraceRange, ECC_Visibility,
            FCollisionQueryParams(FName(), true, this)))
		{
			SetFireRotation((HitResult.Location - GetGunPortLocation()).Rotation());
			return true;
		}
	}
	return false;
}

void AWeapon::CalculateFireDirectionByAdjustmentAlgorithm(const FVector CameraLocation, const FRotator CameraRotation)
{
	SetFireRotation((CameraRotation.RotateVector(FVector::ForwardVector).GetSafeNormal() * 1000 +
        CameraLocation - GetGunPortLocation()).Rotation());
}

float AWeapon::GetAdjustedFireBankAngle()
{
	return FMath::Acos(FVector::DotProduct(GetGunPortDirection().GetSafeNormal(),
		FireDirection.GetSafeNormal()));
}

void AWeapon::ApplyFireDirectionRandomOffset(const float PrecisionRange)
{
	FireDirection = FRotator(
        FMath::FRandRange(-PrecisionRange, PrecisionRange),
        FMath::FRandRange(-PrecisionRange, PrecisionRange),
        FMath::FRandRange(-PrecisionRange, PrecisionRange)
    ).RotateVector(FireDirection);
}

bool AWeapon::IsFiring_Implementation()
{
	return bFiring;
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
	//UE_LOG(LogTemp, Display, TEXT("AWeapon::ActionFire_Implementation"));
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
	//UE_LOG(LogTemp, Display, TEXT("AWeapon::ActionStopFire_Implementation"));
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

