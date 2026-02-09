#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

class URogueSaveSubSystem;

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
	URogueSaveSubSystem* GetSaveSS();

	bool BeSaveSS(URogueSaveSubSystem* savess);


private:
	URogueSaveSubSystem* SaveSS;
};
