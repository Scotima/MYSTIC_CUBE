#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

class URogueSaveSubsystem;

UCLASS()
class DEMONKING_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Save")
	void RequestContinueRun();

	UFUNCTION(BlueprintCallable, Category = "Save")
	void RequestSaveAndLeaveToLobby();

private:
	URogueSaveSubsystem* GetSaveSS();

	bool BeSaveSS(URogueSaveSubsystem* savess);


private:
	URogueSaveSubsystem* SaveSS;
};
