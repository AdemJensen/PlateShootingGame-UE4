
// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Basics/PickableWeapon.h"


#include "Kismet/GameplayStatics.h"
#include "Pickables/HoldableActor.h"

APickableWeapon::APickableWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	PickupRange = CreateDefaultSubobject<USphereComponent>("PickupRange");
	PickupRange->InitSphereRadius(50.0f);
	PickupRange->SetCollisionObjectType(ECC_WorldDynamic);
	PickupRange->SetGenerateOverlapEvents(true);
	PickupRange->SetSimulatePhysics(false);
	PickupRange->SetCollisionResponseToAllChannels(ECR_Overlap);
	PickupRange->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	PickupRange->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	PickupRange->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	PickupRange->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);
	PickupRange->SetupAttachment(RootComponent);
	PickupRange->OnComponentBeginOverlap.AddDynamic(this, &APickableWeapon::OnPickupRangeBeginOverlap);
	PickupRange->OnComponentEndOverlap.AddDynamic(this, &APickableWeapon::OnPickupRangeEndOverlap);

	WeaponBody->SetCollisionObjectType(ECC_PhysicsBody);
	WeaponBody->SetSimulatePhysics(true);
	WeaponBody->SetEnableGravity(true);
	WeaponBody->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WeaponBody->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	WeaponBody->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	WeaponBody->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	WeaponBody->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	WeaponBody->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	WeaponBody->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
	WeaponBody->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Overlap);
	WeaponBody->SetCollisionResponseToChannel(ECC_Destructible, ECR_Overlap);

}

void APickableWeapon::OnPickup_Implementation(AActor* ActionActor)
{
	HandlePickupActionOnServer(ActionActor);
}

void APickableWeapon::OnDrop_Implementation()
{
	HandleDropActionOnServer();
}

FPickableItemDisplayData APickableWeapon::GetPickableInfo_Implementation()
{
	return {};
}

bool APickableWeapon::IsOpenForPickup_Implementation()
{
	return !IsValid(WeaponOwner);
}

void APickableWeapon::AnnounceAbleToPickupOnAll_Implementation()
{
	TArray<AActor*> OverlappedActors;
	GetOverlappingActors(OverlappedActors, UHoldableActor::StaticClass());
	for (AActor* Target : OverlappedActors)
	{
		// This is guaranteed, but still, more check, more safe.
		if (Target->GetClass()->ImplementsInterface(UHoldableActor::StaticClass()))
		{
			IHoldableActor* HoldableActor = Cast<IHoldableActor>(Target);
			HoldableActor->Execute_OnAbleToPickupItem(Target, this);
		}
	}
}

void APickableWeapon::HandleDropActionOnServer_Implementation()
{
	ActionStopFire();
	WeaponOwner = nullptr;
	SetSimulatePhysicsOnAll(true);
	AnnounceAbleToPickupOnAll();
	PlaySoundOnAll(false);
}

void APickableWeapon::PlaySoundOnAll_Implementation(const bool PickupSound)
{
	USoundCue* SoundToPlay = (PickupSound ? PickupSoundEffect : DropSoundEffect);
	UWorld* World = GetWorld();
	if (IsValid(SoundToPlay) && IsValid(World))
	{
		UGameplayStatics::PlaySoundAtLocation(World, SoundToPlay, WeaponBody->GetComponentLocation());
	}
}

void APickableWeapon::AnnounceLostTrackOnAll_Implementation()
{
	TArray<AActor*> OverlappedActors;
	GetOverlappingActors(OverlappedActors, UHoldableActor::StaticClass());
	for (AActor* Target : OverlappedActors)
	{
		// This is guaranteed, but still, more check, more safe.
		if (Target->GetClass()->ImplementsInterface(UHoldableActor::StaticClass()))
		{
			IHoldableActor* HoldableActor = Cast<IHoldableActor>(Target);
			HoldableActor->Execute_OnLostTrackOfItem(Target, this);
		}
	}
}

void APickableWeapon::SetSimulatePhysicsOnAll_Implementation(const bool SimulatePhysics)
{
	WeaponBody->SetSimulatePhysics(SimulatePhysics);
	WeaponBody->SetEnableGravity(SimulatePhysics);
}

void APickableWeapon::HandlePickupActionOnServer_Implementation(AActor* ActionActor)
{
	WeaponOwner = ActionActor;
	SetSimulatePhysicsOnAll(false);
	AnnounceLostTrackOnAll();
	PlaySoundOnAll(true);
}

void APickableWeapon::OnPickupRangeBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent,
                                                               AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                               const FHitResult& SweepResult)
{
	if (Execute_IsOpenForPickup(this) && OtherActor->GetClass()->ImplementsInterface(UHoldableActor::StaticClass()))
	{
		IHoldableActor* HoldableActor = Cast<IHoldableActor>(OtherActor);
		HoldableActor->Execute_OnAbleToPickupItem(OtherActor, this);	// Why? what happens if ues OnAbleToPickupItem directly?
	}
}

void APickableWeapon::OnPickupRangeEndOverlap_Implementation(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Execute_IsOpenForPickup(this) && OtherActor->GetClass()->ImplementsInterface(UHoldableActor::StaticClass()))
	{
		IHoldableActor* HoldableActor = Cast<IHoldableActor>(OtherActor);
		HoldableActor->Execute_OnLostTrackOfItem(OtherActor, this);		// Why? what happens if ues OnLostTrackOfItem directly?
	}
}
