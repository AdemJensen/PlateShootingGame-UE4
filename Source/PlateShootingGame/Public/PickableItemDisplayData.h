#pragma once

#include "PickableItemDisplayData.generated.h"

class UImage;
USTRUCT(BlueprintType)
struct FPickableItemDisplayData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    UObject* Image;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FText Title;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FText Description;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    bool bStackable;
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int32 Quantity;
    
    
};