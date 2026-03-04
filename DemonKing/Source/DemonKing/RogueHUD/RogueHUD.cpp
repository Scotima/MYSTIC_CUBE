#include "DemonKing/RogueHUD/RogueHUD.h"
#include "Blueprint/UserWidget.h"

void ARogueHUD::ClearAllUI()
{
	if (MainMenuWidget != nullptr)
	{
		MainMenuWidget->RemoveFromParent();
		MainMenuWidget = nullptr;
	}

	if (StartGameWidget != nullptr)
	{
		StartGameWidget->RemoveFromParent();
		StartGameWidget = nullptr;
	}
}

void ARogueHUD::ShowMainMenuWidget()
{

	ClearAllUI();

	if (!MainMenuWidgetClass)
	{
		return;
	}

	MainMenuWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass);
	
	if (MainMenuWidget)
	{
		MainMenuWidget->AddToViewport();
	}
}

void ARogueHUD::ShowStartGameWidget()
{
	ClearAllUI();

	if (!StartGameWidgetClass)
	{
		return;
	}

	StartGameWidget = CreateWidget<UUserWidget>(GetWorld(), StartGameWidgetClass);

	if (StartGameWidget)
	{
		StartGameWidget->AddToViewport();
	}
}
