#include "DemonKing/CWidget/LobbyMainWidget.h"
#include "DemonKing/GameFlow/RoguePlayerState.h"
#include "GameFramework/GameStateBase.h"
#include "DemonKing/CWidget/LobbyWidget.h"
#include "Components/VerticalBox.h"
#include "GameFramework/PlayerState.h" 
#include "Components/Button.h"
#include "DemonKing/Online/MySessionSubsystem.h"


void ULobbyMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	mySubsystem = nullptr;

	RefreshPlayerList();

	BackButton->OnClicked.AddDynamic(this, &ULobbyMainWidget::HandleReturnToLobby);
	btn_InviteButton->OnClicked.AddDynamic(this, &ULobbyMainWidget::ShowInviteWidget);
}

void ULobbyMainWidget::RefreshPlayerList()
{
	AGameStateBase* GSB = GetWorld() ? GetWorld()->GetGameState() : nullptr;

	if (!GSB)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ULobbyMainWidget::RefreshPlayerList] !AGameStateBase"));
		return;
	}

	if (!lobbyentryclass)
	{
		return;
	}

	if (!playerList)
	{
		return;
	}

	playerList->ClearChildren();
	
	for (APlayerState* ps : GSB->PlayerArray)
	{
		ARoguePlayerState* RoguePS = Cast<ARoguePlayerState>(ps);

		if (!RoguePS)
		{
			UE_LOG(LogTemp, Warning, TEXT("[ULobbyMainWidget::RefreshPlayerList] !RoguePS"));
			continue;
		}

		ULobbyWidget* lobywidget = CreateWidget<ULobbyWidget>(this, lobbyentryclass);

		if (!lobywidget)
		{
			UE_LOG(LogTemp, Warning, TEXT("[ULobbyMainWidget::RefreshPlayerList] !lobywidget"));
			continue;
		}

		lobywidget->SetUpEntry(RoguePS);

		playerList->AddChildToVerticalBox(lobywidget);
	}
	UE_LOG(LogTemp, Warning, TEXT("[ULobbyMainWidget::RefreshPlayerList]"));
}

void ULobbyMainWidget::HandleReturnToLobby()
{
	mySubsystem = GetMYSS();

	if (mySubsystem)
	{
		mySubsystem->ReturnToLobby();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ULobbyMainWidget::ShowInviteWidget] mySubsystem nullptr"));
		return;
	}
}

void ULobbyMainWidget::ShowInviteWidget()
{
	mySubsystem = GetMYSS();

	if (mySubsystem)
	{
		mySubsystem->ShowInviteUI();
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ULobbyMainWidget::ShowInviteWidget] mySubsystem nullptr"));
		return;
	}
}

UMySessionSubsystem* ULobbyMainWidget::GetMYSS()
{
	return GetGameInstance() ? GetGameInstance()->GetSubsystem<UMySessionSubsystem>() : nullptr;
}


