#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

class URogueSaveSubsystem;

UENUM(BlueprintType)
enum class EPlayerClassType : uint8
{
	Warrior UMETA(DisplayName = "Warrior"),
	Mage UMETA(DisplayName = "Mage"),
	Archer UMETA(DisplayName = "Archer")
};

UCLASS()
class DEMONKING_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Save")
	void RequestContinueRun();

	UFUNCTION(BlueprintCallable, Category = "Save")
	void RequestSaveAndLeaveToLobby();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	EPlayerClassType SelectedPlayerClass = EPlayerClassType::Warrior;

private:
	URogueSaveSubsystem* GetSaveSS();

	bool BeSaveSS(URogueSaveSubsystem* savess);

private:
	URogueSaveSubsystem* SaveSS;
};