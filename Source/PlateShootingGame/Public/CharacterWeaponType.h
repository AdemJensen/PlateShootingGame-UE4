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

    EMPTY = 1                   UMETA(DisplayName = "Empty"),
    ASSAULT_RIFLE = 2           UMETA(DisplayName = "Assualt Rifle"),
    SNIPER_RIFLE = 2            UMETA(DisplayName = "Sniper Rifle"),
    SHORT_GUN = 3               UMETA(DisplayName = "Short Gun"), 
    GRENADE_LAUNCHER = 4        UMETA(DisplayName = "Grenade Launcher"), 
    ROCKET_LAUNCHER = 5         UMETA(DisplayName = "Rocket Launcher (RPG)"), 
    
};
