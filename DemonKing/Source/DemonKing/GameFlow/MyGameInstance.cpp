#include "DemonKing/GameFlow/MyGameInstance.h"
#include "DemonKing/SaveGame/RogueSaveSubsystem.h"
#include "Kismet/GameplayStatics.h"

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


