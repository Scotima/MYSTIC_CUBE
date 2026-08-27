// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CEnemyHealthWidget.generated.h"

/**
 * 
 */
UCLASS()
class DEMONKING_API UCEnemyHealthWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateHealthWidget(float HealthPercent);
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> HealthProgressBar;

};
