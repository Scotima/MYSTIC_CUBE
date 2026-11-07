// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DemonKing/FoxGirl/SkillBaseComponent.h"
#include "FoxSkillMouseLeftComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DEMONKING_API UFoxSkillMouseLeftComponent : public USkillBaseComponent
{
	GENERATED_BODY()

public:
	virtual void PlaySkillMontage() override;

	FORCEINLINE void SetCanInputCombo(bool a) {CanInputCombo = a;}


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Anim)
	TArray<TObjectPtr<UAnimMontage>> ComboMontages;


private:
	bool CanInputCombo = false;
	int32 ComboIndex = 0;

	
};
