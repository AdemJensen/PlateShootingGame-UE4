// Fill out your copyright notice in the Description page of Project Settings.


#include "MySaveGame.h"

void UMySaveGame::InsertPlayerRecordRanking(FString PlayerNickname, int32 Score)
{
    if (Rankings.Contains(PlayerNickname))
    {
        Rankings[PlayerNickname] = std::max(Rankings[PlayerNickname], Score);
    }
    else
    {
        Rankings.Add(PlayerNickname, Score);
    }
}

TArray<FRankingListData> UMySaveGame::GetSortedRankingList(const int32 ItemNumLimitation)
{
    TArray<FRankingListData> TempArray;
    for (auto RankRecord : Rankings)
    {
        TempArray.Add({ RankRecord.Get<0>(), RankRecord.Get<1>() });
    }
    int32 Len = TempArray.Num();
    for (int32 i = 1; i < Len; i++)
    {
        for (int32 j = 0; j < Len - i; j++)
        {
            if (TempArray[j].Score < TempArray[j + 1].Score)
            {
                Swap(TempArray[j], TempArray[j + 1]);
            }
        }
    }
    if (ItemNumLimitation <= 0) return TempArray;
    if (Len > ItemNumLimitation) TempArray.SetNum(ItemNumLimitation);
    return TempArray;
}
