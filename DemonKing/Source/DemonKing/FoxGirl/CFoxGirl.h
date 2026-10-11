// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DemonKing/CCharacter/RogueCharacterBase.h"
#include "CFoxGirl.generated.h"

UENUM(BlueprintType)

enum class ECharacterMode :uint8
{
	Normal UMETA(DisplayName = "Normal"),
	Combat UMETA(DisplayName = "Combat")
};

UCLASS()
class DEMONKING_API ACFoxGirl : public ARogueCharacterBase
{
	GENERATED_BODY()

public:


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterMode")
	ECharacterMode CharacterMode = ECharacterMode::Normal;
	
};
