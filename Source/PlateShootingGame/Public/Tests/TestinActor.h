// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "BlueprintImplementableInterface.h"
#include "GameFramework/Actor.h"
#include "TestinActor.generated.h"

UCLASS()
class PLATESHOOTINGGAME_API ATestinActor : public AActor, public IBlueprintImplementableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestinActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnPickup_Implementation(ACharacter* ActionCharacter) override;

};
