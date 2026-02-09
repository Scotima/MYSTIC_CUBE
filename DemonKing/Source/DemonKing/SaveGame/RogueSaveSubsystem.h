#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RogueSaveSubsystem.generated.h"

class URogueSaveGame;
class APlayerController;
class Acharacter;

UCLASS()
class DEMONKING_API URogueSaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Save")
	bool HasRunSave() const;

	UFUNCTION(BlueprintCallable, Category = "Save")
	URogueSaveGame* LoadRunSave();

	UFUNCTION(BlueprintCallable, Category = "Save")
	bool WriteRunSave(URogueSaveGame* SaveObj);

	UFUNCTION(BlueprintCallable, Category = "Save")
	void DeleteRunSave();

	UFUNCTION(BlueprintCallable, Category = "Save")
	void SaveOnQuit(APlayerController* PC);

	UFUNCTION(BlueprintCallable, Category = "Save")
	bool LoadOrStartNewRun(APlayerController* PC,
		FName& OutStageId,
		int32& OutStageMapIndex,
		int32& OutRunSeed,
		int32& OutStageSeed);

private:

	URogueSaveGame* CapureSnapshot(APlayerController* PC) const;
	void ApplySnapshot(APlayerController* PC, URogueSaveGame* SaveObj);


private:

	UPROPERTY()
	FString SlotName = TEXT("RunSlot");

	UPROPERTY()
	int32 UserIndex = 0;

	UPROPERTY()
	int32 ExpectedSaveVersion = 1;

	UPROPERTY()
	TObjectPtr<URogueSaveGame> CachedSave = nullptr;


	
};
