
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CPauseMenuWidget.generated.h"


UCLASS()
class DEMONKING_API UCPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "Menu")
	void OnLeaveButtonClicked();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> LeaveButton;
};
