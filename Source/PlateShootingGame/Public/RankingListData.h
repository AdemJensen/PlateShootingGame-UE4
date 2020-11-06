#pragma once

#include "RankingListData.generated.h"

USTRUCT(BlueprintType)
struct FRankingListData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString Nickname;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int32 Score;
    
};