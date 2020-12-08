// This is a private project, cannot be put at commercial use.

#pragma once

#include "CoreMinimal.h"
#include "WeaponType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    UNKNOWN = 0                 UMETA(DisplayName = "Unknown"),

    EMPTY = 1                   UMETA(DisplayName = "Bare Hands (Empty)"),
    ASSAULT_RIFLE = 2           UMETA(DisplayName = "Assualt Rifle"),
    SNIPER_RIFLE = 3            UMETA(DisplayName = "Sniper Rifle"),
    SHORT_GUN = 4               UMETA(DisplayName = "Short Gun"), 
    GRENADE_LAUNCHER = 5        UMETA(DisplayName = "Grenade Launcher"), 
    ROCKET_LAUNCHER = 6         UMETA(DisplayName = "Rocket Launcher (RPG)"),
	GRENADE = 7					UMETA(DisplayName = "Grenade"),
	PISTOL = 8					UMETA(DisplayName = "Pistol"),
	BEAM_GUN = 9				UMETA(DisplayName = "Beam Gun"),
	
	ARTILLERY = 20				UMETA(DisplayName = "Heavy Artillery"),
	REMOTE_CONTROLLED = 21		UMETA(DisplayName = "Remote Controlled"), 
	AI_CONTROLLED = 22			UMETA(DisplayName = "AI Controlled")
    
};
