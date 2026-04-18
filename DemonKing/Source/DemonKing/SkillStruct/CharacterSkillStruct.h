#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CharacterSkillStruct.generated.h"

class UAnimMontage;
class UTexture2D;

USTRUCT(BlueprintType)

struct FCharacterSkillStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 SkillID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName Montage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float CoolDown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName Icon;
};