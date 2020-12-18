// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SniperRifle.h"
#include "GameFramework/Character.h"

#include "GeneratedCodeHelpers.h"
#include "Kismet/GameplayStatics.h"

void ASniperRifle::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ASniperRifle, ShootingRange);
	DOREPLIFETIME(ASniperRifle, ShotImpulse);
	DOREPLIFETIME(ASniperRifle, NormalDamagePerFire);
	DOREPLIFETIME(ASniperRifle, CriticalDamagePerFire);
}

ASniperRifle::ASniperRifle()
{
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(RootComponent);
}

// This event is executed on server.
void ASniperRifle::OnFire_Implementation()
{
	Super::OnFire_Implementation();
	//UE_LOG(LogTemp, Warning, TEXT("OnFire_Implementation"));
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
			auto QueryParams = FCollisionQueryParams(FName(), true, this);
			QueryParams.bReturnPhysicalMaterial = true;
			if (World->LineTraceSingleByChannel(HitResult, GunPortLocation + AimingDirection * 6.0,
				GunPortLocation + AimingDirection * ShootingRange, ECC_Visibility, QueryParams))
			{
				UPrimitiveComponent* HitComponent = HitResult.Component.Get();
				AActor* HitActor = HitResult.Actor.Get();
				const FName HitBone = HitResult.BoneName;
				MakeHitEffectOnAll(HitResult.PhysMaterial.Get(), HitResult.Location,
					HitActor, HitComponent, HitBone);
				
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
					IsValid(WeaponOwner) ? WeaponOwner->GetInstigatorController() : nullptr,
					this, UDamageType::StaticClass());
				// Little question here: What is the difference between AActor::GetInstigatorController() and
				// APawn::GetController() ? (F.Y.I: There is no GetController() here in C++ but have both in BP.)
			}
		}
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Actual_Use = 0"));
		ActionStopFire();
	}
}

void ASniperRifle::OnEnterScope_Implementation(AShooterGameCharacterBase* Character, AWeapon* Weapon)
{
	auto PlayerController = Cast<APlayerController>(Character->GetController());
	if (IsValid(PlayerController))
	{
		CurrentScopeFOV = 0;
		Camera->SetFieldOfView(ScopeFOV[CurrentScopeFOV]);
		PlayerController->SetViewTargetWithBlend(this, 0.2);
		OnEnterScope_BP(Character, Weapon);
	}
}

void ASniperRifle::OnAdjustScope_Implementation(AShooterGameCharacterBase* Character, AWeapon* Weapon)
{
	CurrentScopeFOV++;
	if (CurrentScopeFOV >= ScopeFOV.Num()) CurrentScopeFOV = 0;
	Camera->SetFieldOfView(ScopeFOV[CurrentScopeFOV]);
	OnAdjustScope_BP(Character, Weapon);
}

bool ASniperRifle::CanAdjustNow_Implementation(AShooterGameCharacterBase* Character, AWeapon* Weapon)
{
	return CurrentScopeFOV < ScopeFOV.Num() - 1;
}

void ASniperRifle::OnExitScope_Implementation(AShooterGameCharacterBase* Character, AWeapon* Weapon)
{
	auto PlayerController = Cast<APlayerController>(Character->GetController());
	if (IsValid(PlayerController))
	{
		PlayerController->SetViewTargetWithBlend(Character, 0.2);
		OnExitScope_BP(Character, Weapon);
	}
}

void ASniperRifle::SetFireDirectionByCameraParameters(const FVector CameraLocation, const FRotator CameraRotation)
{
	if (!CalculateFireDirectionByLineTrace(CameraLocation, CameraRotation, ShootingRange))
	{
		CalculateFireDirectionByAdjustmentAlgorithm(CameraLocation, CameraRotation);
	}
}

void ASniperRifle::MakeHitEffectOnAll_Implementation(UPhysicalMaterial* HitPhysicalMaterial, FVector HitLocation,
	AActor* HitActor, UPrimitiveComponent* HitComponent, FName HitBoneName)
{
	MakeHitEffect_BP(HitPhysicalMaterial, HitLocation, HitActor, HitComponent, HitBoneName);
}

void ASniperRifle::MakeFireEffectOnAll_Implementation(const bool Fired)
{
	MakeFireEffect_BP(Fired);
}

