// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CCharacterStatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DEMONKING_API UCCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UCCharacterStatComponent();

protected:
	virtual void BeginPlay() override;

public:	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void TakeDamage(float MonsterPower);
	void Die();

	//블루프린트에서도 할 수 있게..
	//공식은 같지만 마법사 체력이 낮고
	//전사니까 체력이 높고


private:
	float MaxHp;
	float CurrentHp;

	float AttackPower;
	
	bool isDead;
	bool bCanBeDamaged;

	float Defense;

		
};
