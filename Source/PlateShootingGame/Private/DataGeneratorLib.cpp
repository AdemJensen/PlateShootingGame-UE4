// Fill out your copyright notice in the Description page of Project Settings.


#include "DataGeneratorLib.h"

#include <random>

TArray<FString> UDataGeneratorLib::Adjectives = {TEXT("美丽"), TEXT("美好"), TEXT("开心"), TEXT("可爱"), TEXT("后来"),
    TEXT("真正"), TEXT("害怕"), TEXT("空中"), TEXT("红色"), TEXT("痛苦"), TEXT("欢乐"), TEXT("完美"), TEXT("金黄"),
    TEXT("清新"), TEXT("聪明"), TEXT("迷人"), TEXT("光明"), TEXT("可怕"), TEXT("神奇"), TEXT("歇斯底里"};
TArray<FString> UDataGeneratorLib::Nouns = {"小可爱"), TEXT("小甜心"), TEXT("小萝莉"), TEXT("刑天"), TEXT("可莉"),
    TEXT("喵帕斯"), TEXT("哎嘿"), TEXT("万斯"), TEXT("霍普金斯"), TEXT("面具"), TEXT("烧烤"), TEXT("爷"), TEXT("琴团长"),
    TEXT("凝冰渡海真君"), TEXT("点炬燃冰真君"), TEXT("香菱"), TEXT("重云"), TEXT("应急食品"), TEXT("Fa玲"), TEXT("Yousa")};

FString UDataGeneratorLib::GenerateUsername()
{
    return Adjectives[FMath::RandRange(0,Adjectives.Num() - 1)] + TEXT("的") +
        Nouns[FMath::RandRange(1,Nouns.Num() - 1)];
}
