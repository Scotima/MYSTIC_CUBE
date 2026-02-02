
#pragma once

#include "CoreMinimal.h"
#include "RogueRunTypes.generated.h"

UENUM(BlueprintType)

enum class ERogueRunEndReason : uint8
{
	None,
	PartyWipe,
	BossKilled,
	Quit,
	Debug
};