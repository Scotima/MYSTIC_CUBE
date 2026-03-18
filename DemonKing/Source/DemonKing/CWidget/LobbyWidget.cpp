#include "DemonKing/CWidget/LobbyWidget.h"
#include "DemonKing/GameFlow/RoguePlayerState.h"
#include "Components/TextBlock.h" 
void ULobbyWidget::NativeDestruct()
{
	if (cachedPlayerState)
	{
		cachedPlayerState->OnLobbyNickNameChanged.RemoveDynamic(this, &ULobbyWidget::HandleNicknameChanged);
	}

	Super::NativeDestruct();
}
void ULobbyWidget::SetUpEntry(ARoguePlayerState* playerstate)
{

	if (!playerstate)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ULobbyWidget::SetUpEntry] playerstate is nullptr"));
		return;
	}



	if (cachedPlayerState)
	{
		cachedPlayerState->OnLobbyNickNameChanged.RemoveDynamic(this, &ULobbyWidget::HandleNicknameChanged);
	}

	cachedPlayerState = playerstate;

	RefreshNickName();


	cachedPlayerState->OnLobbyNickNameChanged.AddDynamic(this, &ULobbyWidget::HandleNicknameChanged);

	UE_LOG(LogTemp, Warning, TEXT("[ULobbyWidget::SetUpEntry] complete!"));
}

void ULobbyWidget::HandleNicknameChanged(const FString& Nick)
{

	if (!Text_Nickname)
	{
		return;
	}

	Text_Nickname->SetText(FText::FromString(Nick));

	UE_LOG(LogTemp, Warning, TEXT("[ULobbyWidget::HandleNicknameChanged] complete!"));

}

void ULobbyWidget::RefreshNickName()
{
	if (!cachedPlayerState || !Text_Nickname)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ULobbyWidget::RefreshNickName] !cachedPlayerState || !Text_Nickname"));
		return;
	}

	Text_Nickname->SetText(FText::FromString(cachedPlayerState->GetLobbyPlayerNickName()));
	UE_LOG(LogTemp, Warning, TEXT("[ULobbyWidget::RefreshNickName] complete!"));

}
