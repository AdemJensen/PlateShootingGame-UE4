// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "RankingListData.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class PLATESHOOTINGGAME_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString LocalGamePlayerNickname;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, int32> Rankings;

public:

	UFUNCTION(BlueprintCallable)
	void InsertPlayerRecordRanking(FString PlayerNickname, int32 Score);

	UFUNCTION(BlueprintCallable)
    TArray<FRankingListData> GetSortedRankingList(int32 ItemNumLimitation);
	
};
