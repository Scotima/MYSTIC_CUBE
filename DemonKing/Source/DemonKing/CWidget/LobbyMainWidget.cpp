#include "DemonKing/CWidget/LobbyMainWidget.h"
#include "DemonKing/GameFlow/RoguePlayerState.h"
#include "GameFramework/GameStateBase.h"
#include "DemonKing/CWidget/LobbyWidget.h"
#include "Components/VerticalBox.h"
#include "GameFramework/PlayerState.h" 

void ULobbyMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RefreshPlayerList();
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
