// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Projectiles/Projectile.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

#include "GeneratedCodeHelpers.h"
#include "Kismet/GameplayStatics.h"

void AProjectile::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AProjectile, MassInKg);
}

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	AActor::SetReplicateMovement(true);

	ProjectileBody = CreateDefaultSubobject<UStaticMeshComponent>("ProjectileBody");
	ProjectileArrow = CreateDefaultSubobject<UArrowComponent>("ProjectileDirection");
	//CollisionBox = CreateDefaultSubobject<UCapsuleComponent>("ProjectileCollision");

	RootComponent = ProjectileBody;
	//CollisionBox->SetupAttachment(RootComponent);
	ProjectileArrow->SetupAttachment(RootComponent);

	ProjectileBody->BodyInstance.bOverrideMass = true;
	ProjectileBody->BodyInstance.SetMassOverride(MassInKg);

}
