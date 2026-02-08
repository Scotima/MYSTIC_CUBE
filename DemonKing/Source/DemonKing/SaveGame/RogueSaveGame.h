#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "RogueSaveGame.generated.h"


UCLASS()
class DEMONKING_API URogueSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 SaveVersion = 1;
	UPROPERTY()
	FName StageId = NAME_None;
	UPROPERTY()
	int32 StageMapIndex = 0;
	UPROPERTY()
	int32 RunSeed = 0;
	UPROPERTY()
	int32 StageSeed = 0;

	UPROPERTY()
	bool bHasSavedTransform = false;

	UPROPERTY()
	FTransform PlayerTransform;

	UPROPERTY()
	int32 PlayerGold;

	UPROPERTY()
	TArray<FName> SkillSlots;


	UPROPERTY()
	TArray<FName> InventoryItemIds;

	UPROPERTY()
	TArray<int32> InventoryItemCounts;

	UPROPERTY()
	TArray<int32> EquippedSlotTypes;

	UPROPERTY()
	TArray<FName> EquippedItemIds;


	
};
