#pragma once

#include "CoreMinimal.h"
#include "GeneralEnumLibrary.generated.h"

/**
 * Inventory objects types
 */
UENUM(BlueprintType)
enum class EObjectCategory : uint8 {
	Consumable,
	Important,
	ProjectyleType,
	Material,
	Other,
	None
};

/**
 * Interactions types
 */
UENUM(BlueprintType)
enum class EInteractionCategory : uint8 {
	Talk,
	Use,
	Open,
	None
};

/**
 * Wheater types
 */
UENUM(BlueprintType)
enum class EWeatherType : uint8 {
	ClearSky,
	PartiallyCloudy,
	Cloudy,
	Overcast,
	Foggy,
	Rain,
	Snow
};