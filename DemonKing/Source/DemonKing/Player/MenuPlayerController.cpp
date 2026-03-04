#include "DemonKing/Player/MenuPlayerController.h"
#include "DemonKing/Online/MySessionSubsystem.h"
#include "DemonKing/GameFlow/RogueGameModeBase.h"
#include "Blueprint/UserWidget.h"


AMenuPlayerController::AMenuPlayerController()
{
	ConstructorHelpers::FClassFinder<UUserWidget> Widget(TEXT("/Game/BPWidget/BP_TestWidget"));

	if (Widget.Succeeded())
	{
		TestWidgetClass = Widget.Class;
	}

	ConstructorHelpers::FClassFinder<UUserWidget> MainWidget(TEXT("/Game/ModularShowcase/Widgets/MainMenu/WBP_MainMenu"));

	if (MainWidget.Succeeded())
	{
		MainMenuWidgetClass = MainWidget.Class;
	}
}

void AMenuPlayerController::BeginPlay()
{
	bShowMouseCursor = true;

	//ShowMenuWidget();

	MakeMultiRoom();

}

void AMenuPlayerController::ShowMenuWidget()
{
	if (MainMenuWidget != nullptr)
	{
		MainMenuWidget->RemoveFromParent();
		MainMenuWidget = nullptr;
		return;
	}

	if (MainMenuWidgetClass == nullptr)
	{
		return;
	}

	MainMenuWidget = CreateWidget(this, MainMenuWidgetClass);
	MainMenuWidget->AddToViewport();
}

void AMenuPlayerController::MakeMultiRoom()
{

	MSS = GetMSS();

	if (!MSS)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't get UMySessionSubsystem"));
		return;
	}

	//MSS->MakeSession();

	bWasSuccessful = MSS->GetStartSessionCompleteSuccessful();


	TestWidget = CreateWidget(this, TestWidgetClass);


	if (TestWidget != nullptr)
	{
		TestWidget->AddToViewport();
	}



	
}

void AMenuPlayerController::FindMultiRoom()
{
	MSS = GetMSS();

	if (!MSS)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't get UMySessionSubsystem"));
		return;
	}

	MSS->FindSession();
}

void AMenuPlayerController::Server_RequestStartRun_Implementation()
{
	if (!HasAuthority())
	{
		return;
	}

	if (!bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartSession not successful"));
		return;
	}

	if (ARogueGameModeBase* GM = GetWorld()->GetAuthGameMode<ARogueGameModeBase>())
	{
		GM->StartRun();
	}
}


UMySessionSubsystem* AMenuPlayerController:: GetMSS()
{
	return GetGameInstance()->GetSubsystem<UMySessionSubsystem>();
}