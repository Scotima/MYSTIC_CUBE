

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "CPlayerHPWidgetComponent.generated.h"


UCLASS()
class DEMONKING_API UCPlayerHPWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void UpdateHealthComponent(float CurrentHp);

protected:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Widget")
	TObjectPtr<class UCPlayerHPWidget> PlayerHPWidget;
	
};
