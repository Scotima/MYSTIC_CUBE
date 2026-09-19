#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

class URogueSaveSubsystem;
class APawn;

UENUM(BlueprintType)
enum class EPlayerClassType : uint8
{
	Warrior UMETA(DisplayName = "Warrior"),
	Mage UMETA(DisplayName = "Mage"),
	Archer UMETA(DisplayName = "Archer"),
	Assasin UMETA(DisplayName = "Assasin")
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

	void SetHP_Percent(APawn* PlayerPawn,float currentHP);

	bool TryGetSavedHPPercent(int32 PlayerId, float& OutHPPercent) const;


private:
	URogueSaveSubsystem* GetSaveSS();

	bool BeSaveSS(URogueSaveSubsystem* savess);

private:
	URogueSaveSubsystem* SaveSS;
	int32 ExpectedPlayerCount = 0;

	float HP_Percent;

	float Health;
	float Mana;

	TMap<int32, float> PlayerHPPercentById;
};