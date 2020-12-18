// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Basics/ProjectileSpawnerWeapon.h"
#include "GameFramework/Character.h"

#include "GeneratedCodeHelpers.h"
#include "Kismet/GameplayStatics.h"

void AProjectileSpawnerWeapon::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AProjectileSpawnerWeapon, InitialProjectileVelocity);
}

AProjectileSpawnerWeapon::AProjectileSpawnerWeapon()
{
	
}

void AProjectileSpawnerWeapon::OnFire_Implementation()
{
	const bool AmmoUsed = UseAmmo(1.0, false) > 0;
	MakeFireEffectOnAll(AmmoUsed);
	UWorld* World = GetWorld();
	if (AmmoUsed && IsValid(World))
	{
		FVector SpawnLocation = GetGunPortLocation();
		FRotator SpawnRotation = GetGunPortRotation();
		AActor* Projectile = World->SpawnActor(ProjectileClass, &SpawnLocation, &SpawnRotation);
		if (ProjectileClass->IsChildOf(AProjectile::StaticClass()))
		{
			ConfigureProjectile(Cast<AProjectile>(Projectile));
			Cast<UPrimitiveComponent>(Projectile->GetRootComponent())->SetPhysicsLinearVelocity(
				Projectile->GetActorRotation().RotateVector(FVector::ForwardVector).GetSafeNormal() *
				InitialProjectileVelocity);
		}
	}
}

void AProjectileSpawnerWeapon::ConfigureProjectile(AProjectile* Projectile)
{
	ConfigureProjectile_BP(Projectile);
	if (IsValid(WeaponOwner) && WeaponOwner->GetClass()->IsChildOf(APawn::StaticClass()))
	{
		Projectile->SetInstigator(Cast<APawn>(WeaponOwner));
	}
}

void AProjectileSpawnerWeapon::SetFireDirectionByCameraParameters(const FVector CameraLocation,
                                                                  const FRotator CameraRotation)
{
	CalculateFireDirectionByAdjustmentAlgorithm(CameraLocation, CameraRotation);
}

void AProjectileSpawnerWeapon::MakeFireEffectOnAll_Implementation(const bool Fired)
{
	MakeFireEffect_BP(Fired);
}
