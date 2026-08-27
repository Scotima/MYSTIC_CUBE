// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CEnemyBase.generated.h"

UCLASS()
class DEMONKING_API ACEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UFUNCTION(BlueprintCallable)
	float Play_AnimMontage();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Anim")
	TObjectPtr<UAnimMontage> AttackAnim;

};
