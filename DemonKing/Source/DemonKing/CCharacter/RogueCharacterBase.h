// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RogueCharacterBase.generated.h"

UCLASS()
class DEMONKING_API ARogueCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARogueCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:
	virtual void InputSkillQ();
	virtual void InputSkillE();
	virtual void InputSkillShift();
	virtual void InputSkillLeftMouse();

	FORCEINLINE void SetUsingSkill(bool a) { bUsingSkill = a; }
	FORCEINLINE bool GetUsingSkill() { return bUsingSkill; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<class USpringArmComponent> SpringArm;


protected:
	bool bUsingSkill = false;
};
