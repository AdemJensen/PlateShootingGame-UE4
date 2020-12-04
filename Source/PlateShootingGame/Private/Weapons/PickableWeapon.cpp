// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/PickableWeapon.h"

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

}

void APickableWeapon::OnPickup_Implementation(AActor* ActionActor)
{
}

void APickableWeapon::OnDrop_Implementation()
{
}

FPickableItemDisplayData APickableWeapon::GetPickableInfo_Implementation()
{
	return {};
}

bool APickableWeapon::IsOpenForPickup_Implementation()
{
	return false;
}

void APickableWeapon::OnPickupRangeBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor->GetClass()->ImplementsInterface(UHoldableActor::StaticClass()))
	{
		if (ActorsWaitingToPickMeUp.AddUnique(OtherActor) != INDEX_NONE && !IsValid(WeaponOwner))
		{
			IHoldableActor* HoldableActor = Cast<IHoldableActor>(OtherActor);
			HoldableActor->Execute_OnAbleToPickupItem(OtherActor, this);	// Why? what happens if ues OnAbleToPickupItem directly?
		}
	}
}

void APickableWeapon::OnPickupRangeEndOverlap_Implementation(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->GetClass()->ImplementsInterface(UHoldableActor::StaticClass()))
	{
		if (ActorsWaitingToPickMeUp.Remove(OtherActor))
		{
			IHoldableActor* HoldableActor = Cast<IHoldableActor>(OtherActor);
			HoldableActor->Execute_OnLostTrackOfItem(OtherActor, this);	// Why? what happens if ues OnLostTrackOfItem directly?
		}
	}
}
