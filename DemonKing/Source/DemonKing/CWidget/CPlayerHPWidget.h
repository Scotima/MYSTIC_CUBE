
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CPlayerHPWidget.generated.h"

UCLASS()
class DEMONKING_API UCPlayerHPWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void UpdatePlayerHealth(float CurrentHp);


protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> HealthBar;
	
};
