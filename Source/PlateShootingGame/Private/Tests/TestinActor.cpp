// Fill out your copyright notice in the Description page of Project Settings.


#include "Tests/TestinActor.h"

// Sets default values
ATestinActor::ATestinActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestinActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestinActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestinActor::OnPickup_Implementation(ACharacter* ActionCharacter)
{
	//UE_LOG(LogTemp, Warning, TEXT("Executing OnPickup_Native"));
}
