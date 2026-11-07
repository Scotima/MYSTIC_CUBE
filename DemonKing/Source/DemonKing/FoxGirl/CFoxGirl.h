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

class UChildActorComponent;
UCLASS()
class DEMONKING_API ACFoxGirl : public ARogueCharacterBase
{
	GENERATED_BODY()


public:
	ACFoxGirl(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void BeginPlay() override;

	virtual void OnSkillMontageEnded(UAnimMontage* Montage, bool bInterrupted) override;

public:



	virtual void InputSkillQ() override;
	virtual void InputSkillE() override;
	virtual void InputSkillShift() override;
	virtual void InputSkillLeftMouse() override;


public:
	UFUNCTION(BlueprintCallable, Category = "weapon")
	bool AttachKatanaToSocket(UChildActorComponent* KatanaComponent,
	FName SocketName, FVector LocationOffset, FRotator RotationOffset);

	UFUNCTION(BlueprintPure, Category = "weapon")
	class ACKatanaComponent* GetKatanaComponent() const { return KatanaComponent.Get(); }


public:


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterMode")
	ECharacterMode CharacterMode = ECharacterMode::Normal;

protected:

	UPROPERTY(Transient)
	TObjectPtr<class ACKatanaComponent> KatanaComponent;
	
};
