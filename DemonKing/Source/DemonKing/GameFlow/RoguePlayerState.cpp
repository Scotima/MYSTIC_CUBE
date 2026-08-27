#include "DemonKing/GameFlow/RoguePlayerState.h"
#include "Net/UnrealNetwork.h"
ARoguePlayerState::ARoguePlayerState()
{
	bReplicates = true;
}

void ARoguePlayerState::SetLobbyPlayerNickName(FString& NickName)
{
	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("[ARoguePlayerState::SetLobbyPlayerNickName] !HasAuthority"));
		return;
	}
	PlayerNickName = NickName;
}

FString ARoguePlayerState::GetLobbyPlayerNickName()
{
	return PlayerNickName;
}

void ARoguePlayerState::SetPlayerState_HP(float hp)
{
	if (!HasAuthority())
	{
		return;
	}

	const float NewHpPercent = FMath::Clamp(hp, 0.0f, 1.0f);

	if (FMath::IsNearlyEqual(CurrentHpPercent, NewHpPercent))
	{
		return;
	}

	CurrentHpPercent = NewHpPercent;
	OnPlayerHpChanged.Broadcast(CurrentHpPercent);

}

void ARoguePlayerState::OnRep_PlayerNickName()
{
	OnLobbyNickNameChanged.Broadcast(PlayerNickName);
}

void ARoguePlayerState::OnRep_PlayerHP()
{
	UE_LOG(LogTemp, Warning, TEXT("[ARoguePlayerState] :: OnRep_PlayerHP"));
	OnPlayerHpChanged.Broadcast(CurrentHpPercent);
}

void ARoguePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARoguePlayerState, PlayerNickName);

	DOREPLIFETIME(ARoguePlayerState, CurrentHpPercent);
}
