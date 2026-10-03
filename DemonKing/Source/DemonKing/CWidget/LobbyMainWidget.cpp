#include "DemonKing/CWidget/LobbyMainWidget.h"
#include "DemonKing/GameFlow/RoguePlayerState.h"
#include "GameFramework/GameStateBase.h"
#include "DemonKing/CWidget/LobbyWidget.h"
#include "Components/VerticalBox.h"
#include "GameFramework/PlayerState.h" 
#include "Components/Button.h"
#include "DemonKing/Online/MySessionSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "DemonKing/GameFlow/RogueGameState.h"


void ULobbyMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	mySubsystem = nullptr;



	ARogueGameState* GS = GetWorld() ? GetWorld()->GetGameState<ARogueGameState>() : nullptr;

	if (GS)
	{
		GS->OnLobbyPlayerListChanged.RemoveAll(this);
		GS->OnLobbyPlayerListChanged.AddUObject(this, &ULobbyMainWidget::RefreshPlayerList);
	}

	RefreshPlayerList();

	BackButton->OnClicked.AddDynamic(this, &ULobbyMainWidget::HandleReturnToLobby);
	btn_InviteButton->OnClicked.AddDynamic(this, &ULobbyMainWidget::ShowInviteWidget);
	SelectCharacterButton->OnClicked.AddDynamic(this,&ULobbyMainWidget::ShowCharacterSelectWidget);
}

void ULobbyMainWidget::RefreshPlayerList()
{
	AGameStateBase* GSB = GetWorld() ? GetWorld()->GetGameState() : nullptr;

	UE_LOG(LogTemp, Warning, TEXT("[Lobby] Client=%d, PlayerCount=%d"),
		GetWorld() && GetWorld()->GetNetMode() == NM_Client,
		GSB ? GSB->PlayerArray.Num() : -1);

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

void ULobbyMainWidget::ShowCharacterSelectWidget()
{
	APlayerController* PC = GetOwningPlayer();

	if (!PC || !CharacterSelectWidegetClass)
	{
		return;
	}

	UUserWidget* CharacterSelectWidget = CreateWidget<UUserWidget>(PC, CharacterSelectWidegetClass);

	if (!CharacterSelectWidget)
	{
		return;
	}

	CharacterSelectWidget->AddToViewport(10);
}

UMySessionSubsystem* ULobbyMainWidget::GetMYSS()
{
	return GetGameInstance() ? GetGameInstance()->GetSubsystem<UMySessionSubsystem>() : nullptr;
}


