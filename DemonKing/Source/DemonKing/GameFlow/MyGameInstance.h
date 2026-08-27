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

	UFUNCTION(BlueprintCallable, Category = "PlayerNum")
	void SetExpectedStagePlayerCount(int32 PlayerNum);
	
	FORCEINLINE int32 GetExpectedStagePlayerCount() { return ExpectedPlayerCount; }

private:
	URogueSaveSubsystem* GetSaveSS();

	bool BeSaveSS(URogueSaveSubsystem* savess);

private:
	URogueSaveSubsystem* SaveSS;
	int32 ExpectedPlayerCount = 0;
};