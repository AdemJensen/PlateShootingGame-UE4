// This is a private project, cannot be put at commercial use.

#pragma once

#include "CoreMinimal.h"
#include "CharacterWeaponType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ECharacterWeaponType : uint8
{
    UNKNOWN = 0                 UMETA(DisplayName = "Unknown"),
    // 1 ~ 50 = before actual game. (Preserved)

    // 51 ~ 100 ingame process.
    EMPTY = 1                   UMETA(DisplayName = "Empty"),
    RIFLE = 2                   UMETA(DisplayName = "Rifle"),
    SHORT_GUN = 3               UMETA(DisplayName = "Short Gun")
    
};
