// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillBaseComponent.generated.h"


class UAnimMontage;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DEMONKING_API USkillBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkillBaseComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void PlaySkillMontage();


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Anim")
	TObjectPtr<UAnimMontage> SkillMontage = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<class ARogueCharacterBase> OwnerCharacter = nullptr;

		
};
