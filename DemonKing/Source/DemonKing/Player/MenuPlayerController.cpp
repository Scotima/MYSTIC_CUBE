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
}

void AMenuPlayerController::BeginPlay()
{
	bShowMouseCursor = true;

	TestWidget = CreateWidget(this, TestWidgetClass);
	
	TestWidget->AddToViewport();
}

void AMenuPlayerController::MakeMultiRoom()
{
	MSS = GetMSS();

	if (!MSS)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't get UMySessionSubsystem"));
		return;
	}

	MSS->MakeSession();

	bWasSuccessful = MSS->GetStartSessionCompleteSuccessful();

	
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