

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
	void ShowSkillBarHUD();
	void ShowPauseMenuWidget();

	FORCEINLINE void SetOnOFF(bool a) { OnOFF = a; }
	FORCEINLINE bool GetOnOFF() { return OnOFF; }

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> StartGameWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> SkillBarWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PauseMenuWidgetClass;
private:
	UPROPERTY()
	UUserWidget* MainMenuWidget;

	UPROPERTY()
	UUserWidget* StartGameWidget;

	UPROPERTY()
	UUserWidget* SkillBarWidget;

	UPROPERTY()
	UUserWidget* PauseMenuWidget;

private:
	bool OnOFF = false;
};
