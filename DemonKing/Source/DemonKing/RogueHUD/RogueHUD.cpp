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

	if (SkillBarWidget != nullptr)
	{
		SkillBarWidget->RemoveFromParent();
		SkillBarWidget = nullptr;
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

void ARogueHUD::ShowSkillBarHUD()
{
	ClearAllUI();
	if (!SkillBarWidgetClass)
	{
		return;
	}

	SkillBarWidget = CreateWidget<UUserWidget>(GetWorld(), SkillBarWidgetClass);

	if (SkillBarWidget)
	{
		SkillBarWidget->AddToViewport();
	}


}

void ARogueHUD::ShowPauseMenuWidget()
{
	

	APlayerController* PC = GetOwningPlayerController();

	if (!PC || !PC->IsLocalController() || !PauseMenuWidgetClass)
	{
		return;
	}

	if (OnOFF == true)
	{
		if (IsValid(PauseMenuWidget))
		{
			UE_LOG(LogTemp, Warning, TEXT("ARogueHUD OnOFF = %d"), OnOFF);
			PauseMenuWidget->RemoveFromParent();
			PauseMenuWidget = nullptr;
			return;
		}
	}


	

	PauseMenuWidget = CreateWidget<UUserWidget>(PC, PauseMenuWidgetClass);



	if (IsValid(PauseMenuWidget) && !PauseMenuWidget->IsInViewport())
	{
		PauseMenuWidget->AddToViewport(100);
	}
}
