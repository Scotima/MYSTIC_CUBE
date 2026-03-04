

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RogueHUD.generated.h"

UCLASS()
class DEMONKING_API ARogueHUD : public AHUD
{
	GENERATED_BODY()
	


public:
	void ClearAllUI();
	void ShowMainMenuWidget();
	void ShowStartGameWidget();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> StartGameWidgetClass;
private:
	UPROPERTY()
	UUserWidget* MainMenuWidget;

	UPROPERTY()
	UUserWidget* StartGameWidget;
};
