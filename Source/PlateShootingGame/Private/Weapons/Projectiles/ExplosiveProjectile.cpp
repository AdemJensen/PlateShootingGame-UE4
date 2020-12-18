// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Projectiles/ExplosiveProjectile.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GeneratedCodeHelpers.h"
#include "Kismet/GameplayStatics.h"

void AExplosiveProjectile::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AExplosiveProjectile, bDestructible);
	DOREPLIFETIME(AExplosiveProjectile, Health);
	DOREPLIFETIME(AExplosiveProjectile, DamageRange);
	DOREPLIFETIME(AExplosiveProjectile, ImpulseRange);
	DOREPLIFETIME(AExplosiveProjectile, MaxImpulse);
	DOREPLIFETIME(AExplosiveProjectile, MaxDamage);
}

AExplosiveProjectile::AExplosiveProjectile()
{	
	ProjectileBody->SetSimulatePhysics(true);
	ProjectileBody->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	//CollisionBox->SetSimulatePhysics(true);
	//CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AExplosiveProjectile::BeginPlay()
{
	Super::BeginPlay();
	ProjectileBody->OnComponentHit.AddDynamic(this, &AExplosiveProjectile::OnHit_OnServer);
}

void AExplosiveProjectile::ExcludeActorFromCurrentImpulseCalculationRound(AActor* ActorToBan)
{
	ActorsExcludedFromImpulseCalculation.Add(ActorToBan);
}

void AExplosiveProjectile::OnApplyImpulse(AActor* ActorToApplyImpulse, UPrimitiveComponent* ComponentToApplyImpulse,
	float Distance)
{
	if (ActorToApplyImpulse->GetClass()->IsChildOf(ACharacter::StaticClass()))
	{
		ApplyImpulseOnCharacter(Cast<ACharacter>(ActorToApplyImpulse));
		ExcludeActorFromCurrentImpulseCalculationRound(ActorToApplyImpulse);
	}
	else
	{
		ApplyImpulseOnComponent(ComponentToApplyImpulse);
	}
}

void AExplosiveProjectile::OnHit_OnAll_Implementation(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	OnHit_BP_OnAll(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
}

void AExplosiveProjectile::OnHit_OnServer(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UWorld* World = GetWorld();
	if (IsValid(World) && HasAuthority())
	{
		OnHit_BP_OnServer(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
        OnHit_OnAll(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
	}
}

float AExplosiveProjectile::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	if (Health <= 0)
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
			OnDestroyed_OnServer();
		}
	}
	if (IsValid(KillerCharacter) && KillerCharacter->GetClass()->IsChildOf(StaticClass()))
	{
		Cast<AShooterGameCharacterBase>(KillerCharacter)->OnConfirmedHit(this);
	}
	return DamageAmount;
}

void AExplosiveProjectile::ApplyImpulseOnComponent(UPrimitiveComponent* ComponentToApplyImpulse)
{
	ComponentToApplyImpulse->AddRadialImpulse(GetRootComponent()->GetComponentLocation(), ImpulseRange,
		MaxImpulse, RIF_Constant);
}

void AExplosiveProjectile::ApplyImpulseOnCharacter(ACharacter* CharacterToApplyImpulse)
{
	if (CharacterToApplyImpulse->GetMesh()->IsSimulatingPhysics())	// The Character is dead
	{
		ApplyImpulseOnComponent(CharacterToApplyImpulse->GetMesh());
	}
	else
	{
		CharacterToApplyImpulse->GetCharacterMovement()->AddImpulse(
			CalculateExplosionImpulseForLiveCharacter(
				CharacterToApplyImpulse->GetCapsuleComponent()->GetComponentLocation(),
				GetRootComponent()->GetComponentLocation(), 6
			)
		);
	}
}

FVector AExplosiveProjectile::CalculateExplosionImpulseForLiveCharacter(const FVector CharacterLocation,
                                                                        const FVector ExplodeLocation, const float ImpulseStrengthFactor)
{
	return FRotator(0, 0, 45).RotateVector(FVector(CharacterLocation.X, CharacterLocation.Y, 0) *
		(1 - (CharacterLocation - ExplodeLocation).Size() / ImpulseRange) * ImpulseStrengthFactor);
}

void AExplosiveProjectile::Explode_OnAll_Implementation()
{
	Explode_BP_OnAll();
}

void AExplosiveProjectile::OnDestroyed_OnAll_Implementation()
{
	OnDestroyed_BP_OnAll();
}

void AExplosiveProjectile::OnDestroyed_OnServer_Implementation()
{
	OnDestroyed_BP_OnServer();
	OnDestroyed_OnAll();
}

void AExplosiveProjectile::Explode_OnServer_Implementation()
{
	if (bExploding) return;
	bExploding = true;
	Explode_BP_OnServer();
	Explode_OnAll();
	UWorld* World = GetWorld();

	// Apply Damage on server
	// UGameplayStatics::ApplyRadialDamageWithFalloff(World, MaxDamage, MinDamage,
	// 	GetRootComponent()->GetComponentLocation(),DamageRange * 0.4, DamageRange,
	// 	0.6, nullptr, TArray<AActor*>(), this, GetInstigatorController());
	
	// UGameplayStatics::ApplyRadialDamage(World, MaxDamage, GetRootComponent()->GetComponentLocation(),
	// 	DamageRange, nullptr, TArray<AActor*>(), this,
	// 	GetInstigatorController(), false);

	// Hand-implemented damage in radius
	if (IsValid(World))
	{
		TArray<FHitResult> HitResults;
		World->SweepMultiByChannel(HitResults, GetRootComponent()->GetComponentLocation(),
            GetRootComponent()->GetComponentLocation(), FQuat::Identity, ECC_Camera,
            FCollisionShape::MakeSphere(DamageRange),
            FCollisionQueryParams(NAME_None, false, this));
		ActorsExcludedFromImpulseCalculation.Empty();
		for (FHitResult HitResult : HitResults)
		{
			if (ActorsExcludedFromImpulseCalculation.Contains(HitResult.GetActor()))
			{
				continue;
			}
			UGameplayStatics::ApplyDamage(HitResult.GetActor(),
				FMath::Lerp(MaxDamage, MinDamage,
					FVector::Distance(GetRootComponent()->GetComponentLocation(),
						HitResult.GetActor()->GetActorLocation()) / DamageRange),
				GetInstigatorController(), this, nullptr);
			ExcludeActorFromCurrentImpulseCalculationRound(HitResult.GetActor());
		}
	}
	
	
	// Apply Impulse on server.
	if (IsValid(World))
	{
		TArray<FHitResult> HitResults;
		World->SweepMultiByChannel(HitResults, GetRootComponent()->GetComponentLocation(),
            GetRootComponent()->GetComponentLocation(), FQuat::Identity, ECC_Camera,
            FCollisionShape::MakeSphere(ImpulseRange),
            FCollisionQueryParams(NAME_None, false, this));
		ActorsExcludedFromImpulseCalculation.Empty();
		for (FHitResult HitResult : HitResults)
		{
			if (ActorsExcludedFromImpulseCalculation.Contains(HitResult.GetActor()))
			{
				continue;
			}
			OnApplyImpulse(HitResult.GetActor(), HitResult.GetComponent(), HitResult.Distance);
		}
	}

	Destroy();
}
