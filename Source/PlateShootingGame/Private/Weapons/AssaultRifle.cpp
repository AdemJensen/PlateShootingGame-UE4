// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/AssaultRifle.h"
#include "GameFramework/Character.h"

#include "GeneratedCodeHelpers.h"
#include "Kismet/GameplayStatics.h"

void AAssaultRifle::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AAssaultRifle, ShootingRange);
	DOREPLIFETIME(AAssaultRifle, ShotImpulse);
	DOREPLIFETIME(AAssaultRifle, NormalDamagePerFire);
	DOREPLIFETIME(AAssaultRifle, CriticalDamagePerFire);
}

// This event is executed on server.
void AAssaultRifle::OnFire_Implementation()
{
	const float ActualUse = UseAmmo(1, false);
	MakeFireEffectOnAll(ActualUse > 0);
	UWorld* World = GetWorld();
	if (ActualUse > 0)
	{
		if (IsValid(World))
		{
			FHitResult HitResult;
			const FVector GunPortLocation = WeaponFireArrow->GetComponentLocation();
			const FVector AimingDirection = FireDirection.GetSafeNormal();
			if (World->LineTraceSingleByChannel(HitResult, GunPortLocation + AimingDirection * 6.0,
				GunPortLocation + AimingDirection * ShootingRange, ECC_Visibility))
			{
				UPrimitiveComponent* HitComponent = HitResult.Component.Get();
				AActor* HitActor = HitResult.Actor.Get();
				const FName HitBone = HitResult.BoneName;
				MakeHitEffectOnAll(HitResult.PhysMaterial.Get(), HitActor, HitComponent, HitBone);
				
				// Apply impulse on server
				// This has been changed from the original blue print version, because all the object's movements
				// has been replicated RELIABLE to all, therefore there is no need for replicating impulse
				// to all clients.
				if (HitComponent->IsSimulatingPhysics() && HitComponent->Mobility == EComponentMobility::Movable)
				{
					HitComponent->AddImpulseAtLocation(HitResult.Normal * ShotImpulse,
						HitResult.ImpactPoint, HitBone);
				}

				// Apply Damage
				float DamageToApply;
				ACharacter* HitCharacter = Cast<ACharacter>(HitActor);
				if (IsValid(HitCharacter))	// Damage to Character
				{
					if (HitBone == FName("neck_01") || HitBone == FName("head") ||
						HitCharacter->GetMesh()->BoneIsChildOf(HitBone, FName("neck_01")) ||
						HitCharacter->GetMesh()->BoneIsChildOf(HitBone, FName("head")))
					{
						DamageToApply = CriticalDamagePerFire;
					}
					else
					{
						DamageToApply = NormalDamagePerFire;
					}
				}
				else	// Normal Actor
				{
					DamageToApply = NormalDamagePerFire;
				}
				UGameplayStatics::ApplyDamage(HitActor, DamageToApply * DamageRate,
					WeaponOwner->GetInstigatorController(), this, UDamageType::StaticClass());
				// Little question here: What is the difference between AActor::GetInstigatorController() and
				// APawn::GetController() ? (F.Y.I: There is no GetController() here in C++ but have both in BP.)
			}
		}
	}
	else
	{
		ActionStopFire();
	}
}

void AAssaultRifle::MakeHitEffectOnAll_Implementation(UPhysicalMaterial* HitPhysicalMaterial, AActor* HitActor,
        UPrimitiveComponent* HitComponent, FName HitBoneName)
{
	MakeHitEffect_BP(HitPhysicalMaterial, HitActor, HitComponent, HitBoneName);
}

void AAssaultRifle::MakeFireEffectOnAll_Implementation(const bool Fired)
{
	MakeFireEffect_BP(Fired);
}

