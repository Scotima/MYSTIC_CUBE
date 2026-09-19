
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "StageMonsterInformation.generated.h"


USTRUCT(BlueprintType)

struct FStageMonsterInformation : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stage")
	int32 StageIndex = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stage")
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stage")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stage")
	int32 MonsterCount;

};
