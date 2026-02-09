#include "DemonKing/GameFlow/MyGameInstance.h"
#include "DemonKing/SaveGame/RogueSaveSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UMyGameInstance::RequestContinueRun()
{
	SaveSS = GetSaveSS();

	if (!BeSaveSS(SaveSS))
	{
		//UE_LOG(LogTemp)
	}


}

void UMyGameInstance::RequestSaveAndLeaveToLobby()
{
}

URogueSaveSubSystem* UMyGameInstance::GetSaveSS()
{
	//return GetSubsystem<URogueSaveSubSystem>();
	return nullptr;
}

bool UMyGameInstance::BeSaveSS(URogueSaveSubSystem* savess)
{
	if (savess == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT(" "))
	}

	return true;
}


