// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DT_KnightBoss.generated.h"

class UAnimMontage;

USTRUCT(BlueprintType)
struct DEMONKING_API FDT_KnightBoss : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> AnimMontage;

	// 나중에 데미지 처리하기.
};
