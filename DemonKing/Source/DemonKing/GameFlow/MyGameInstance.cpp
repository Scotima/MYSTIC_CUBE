#include "DemonKing/GameFlow/MyGameInstance.h"
#include "DemonKing/SaveGame/RogueSaveSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "DemonKing/GameFlow/RoguePlayerState.h"

void UMyGameInstance::RequestContinueRun()
{
	SaveSS = GetSaveSS();

	if (!BeSaveSS(SaveSS))
	{
		return;
	}

	APlayerController* PC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;
	FName StageId;
	int32 StageMapIndex;
	int32 RunSeed;
	int32 StageSeed;

	if (SaveSS->LoadOrStartNewRun(PC, StageId, StageMapIndex, RunSeed, StageSeed))
	{
		UE_LOG(LogTemp, Warning, TEXT("SaveLoadTrue"));
	}


}

void UMyGameInstance::RequestSaveAndLeaveToLobby()
{
	APlayerController* PC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;

	SaveSS = GetSaveSS();

	SaveSS->SaveOnQuit(PC);

	UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/Maps/L_MainMenu"));
}

void UMyGameInstance::SetPlayerClassById(const FString& PlayerKey, EPlayerClassType PlayerClass)
{
	PlayerClassById.Add(PlayerKey, PlayerClass);
}

bool UMyGameInstance::TryGetPlayerClassById(const FString& PlayerKey, EPlayerClassType& OutPlayerClass) const
{
	if (PlayerKey.IsEmpty())
	{
		return false;
	}

	const EPlayerClassType* SavedClass = PlayerClassById.Find(PlayerKey);

	if (!SavedClass)
	{
		return false;
	}

	OutPlayerClass = *SavedClass;
	return true;
}

void UMyGameInstance::SetExpectedStagePlayerCount(int32 PlayerNum)
{
	ExpectedPlayerCount = PlayerNum;
}

void UMyGameInstance::SetHP_Percent(APawn* PlayerPawn,float currentHP)
{
	if (!IsValid(PlayerPawn) || !PlayerPawn->HasAuthority())
	{
		return;
	}

	HP_Percent = currentHP;

	ARoguePlayerState* PS = Cast<ARoguePlayerState>(PlayerPawn->GetPlayerState());

	if (!PS)
	{
		return;
	}

	const float HpPercent = FMath::Clamp(currentHP, 0.0f, 1.0f);

	PS->SetPlayerState_HP(HP_Percent);

	const FUniqueNetIdRepl& UniqueId = PS->GetUniqueId();
	if (!UniqueId.IsValid())
	{
		return;
	}

	FString PlayerKey = UniqueId.ToString();

	PlayerHPPercentById.Add(PlayerKey, HpPercent);

}

bool UMyGameInstance::TryGetSavedHPPercent(const FString& PlayerKey, float& OutHPPercent) const
{
	const float* SavedHPPercent = PlayerHPPercentById.Find(PlayerKey);

	if (SavedHPPercent == nullptr)
	{
		return false;
	}

	OutHPPercent = *SavedHPPercent;

	return true;
}

URogueSaveSubsystem* UMyGameInstance::GetSaveSS()
{
	return GetSubsystem<URogueSaveSubsystem>();
	
}

bool UMyGameInstance::BeSaveSS(URogueSaveSubsystem* savess)
{
	if (savess == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("SaveSS is nullptr"));
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("SaveSS exist"));
	
	return true;


}


