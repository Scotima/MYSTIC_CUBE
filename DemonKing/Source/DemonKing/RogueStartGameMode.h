#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DemonKing/GameFlow/RogueGameModeBase.h"
#include "RogueStartGameMode.generated.h"

class AController;
class APawn;
class AActor;
class APlayerController;

UCLASS()
class DEMONKING_API ARogueStartGameMode : public ARogueGameModeBase
{
	GENERATED_BODY()

public:
	ARogueStartGameMode();

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	virtual void BeginPlay() override;

	UFUNCTION()
	void TrySpawnPendingPlayer();

	bool FindGroundedSpawnTransform(FTransform& OutTransform) const;


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
	float TraceStartZOffset = 500.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
	float TraceEndZOffset = 5000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
	float SpawnZOffset = 300.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
	float SpawnRetryInterval = 0.05f;

	UPROPERTY()
	TObjectPtr<APlayerController> PendingPlayerController;

	FTimerHandle SpawnRetryTimerHandle;

protected:
	TSubclassOf<APawn> GetSelectedPawnClass() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Classes")
	TSubclassOf<APawn> WarriorPawnClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Classes")
	TSubclassOf<APawn> MagePawnClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Classes")
	TSubclassOf<APawn> ArcherPawnClass;
};