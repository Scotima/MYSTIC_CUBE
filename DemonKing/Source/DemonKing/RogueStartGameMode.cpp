#include "RogueStartGameMode.h"
#include "GameFlow/MyGameInstance.h"

#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"
#include "DemonKing/GameFlow/RogueGameState.h"

ARogueStartGameMode::ARogueStartGameMode()
{
	//todo HandleStartingNvewPlayer_Implementation(APlayerController* NewPlayer) 이걸로 실행시켜보기.
}

void ARogueStartGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	
}

void ARogueStartGameMode::BeginPlay()
{

}


void ARogueStartGameMode::PostLogin(APlayerController* NewPlayer)
{
	UE_LOG(LogTemp, Warning, TEXT("[ARogueStartGameMode::HandleStartingNewPlayer_Implementation]"));


	if (!NewPlayer)
	{
		return;
	}

	PendingPlayerController.AddUnique(NewPlayer);
	

	// 이미 타이머가 돌고 있지 않으면 시작
	if (!GetWorldTimerManager().IsTimerActive(SpawnRetryTimerHandle))
	{
		GetWorldTimerManager().SetTimer(
			SpawnRetryTimerHandle,
			this,
			&ARogueStartGameMode::TrySpawnPendingPlayer,
			SpawnRetryInterval,
			true
		);
	}
}

void ARogueStartGameMode::Logout(AController* Exiting)
{
	PendingPlayerController.RemoveAll([Exiting](const TWeakObjectPtr<APlayerController>& PendingPlayer)
		{
			return !PendingPlayer.IsValid() || PendingPlayer.Get() == Exiting;
		}
	);

	if (PendingPlayerController.IsEmpty())
	{
		GetWorldTimerManager().ClearTimer(SpawnRetryTimerHandle);

	}

	Super::Logout(Exiting);
}

void ARogueStartGameMode::InitGameState()
{
	Super::InitGameState();

	ARogueGameState* GS =
		GetGameState<ARogueGameState>();

	if (!GS)
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("RogueStartGameMode: GameState is null")
		);
		return;
	}

	GS->SetRunActive(true);
}




void ARogueStartGameMode::PostLogin(APlayerController* NewPlayer)
{
	
}

void ARogueStartGameMode::TrySpawnPendingPlayer()
{

	for (int32 Index = PendingPlayerController.Num() - 1; Index >= 0; --Index)
	{
		APlayerController* PlayerController = PendingPlayerController[Index].Get();

		if (!IsValid(PlayerController))
		{
			PendingPlayerController.RemoveAt(Index);
			continue;
		}

		if (IsValid(PlayerController->GetPawn()))
		{
			PendingPlayerController.RemoveAt(Index);
			continue;
		}


		RestartPlayer(PlayerController);

		if (IsValid(PlayerController->GetPawn()))
		{
			PendingPlayerController.RemoveAt(Index);

		}
	}

	if (PendingPlayerController.Num() == 0)
	{
		GetWorldTimerManager().ClearTimer(SpawnRetryTimerHandle);
		OnSpawnQueueDrained();
	}

	
}

AActor* ARogueStartGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	

	return Super::ChoosePlayerStart_Implementation(Player);
}

bool ARogueStartGameMode::FindGroundedSpawnTransform(FTransform& OutTransform) const
{
	UWorld* world = GetWorld();

	if (!world)
	{
		return false;
	}

	const FVector BaseLocation = OutTransform.GetLocation();

	const FVector TraceStart = BaseLocation + FVector(0.0f, 0.0f, TraceStartZOffset);
	const FVector TraceEnd = BaseLocation - FVector(0.0f, 0.0f, TraceEndZOffset);

	FHitResult HitResult;
	FCollisionQueryParams queryparams;
	queryparams.AddIgnoredActor(this);

	if (!world->LineTraceSingleByChannel(
	HitResult, TraceStart, TraceEnd, ECC_Visibility, queryparams))
	{
		return false;
	}

	OutTransform.SetLocation(HitResult.ImpactPoint + FVector(0.0f, 0.0f, SpawnZOffset));

	return true;
}

APawn* ARogueStartGameMode::SpawnDefaultPawnAtTransform_Implementation(
	AController* NewPlayer,
	const FTransform& SpawnTransform)
{
	UWorld* World = GetWorld();
	
	if (!World)
	{
		return nullptr;
	}

	FTransform FinalTransform = SpawnTransform;
	
	if (!FindGroundedSpawnTransform(FinalTransform))
	{
		return nullptr;
	}


	TSubclassOf<APawn> SelectedPawnClass = GetSelectedPawnClass();

	if (!SelectedPawnClass)
	{
		return Super::SpawnDefaultPawnAtTransform_Implementation(NewPlayer, FinalTransform);
	}




	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = NewPlayer;
	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	APawn* SpawnedPawn = World->SpawnActor<APawn>(
		SelectedPawnClass,
		FinalTransform,
		SpawnParams
	);

	return SpawnedPawn;
}

TSubclassOf<APawn> ARogueStartGameMode::GetSelectedPawnClass() const
{
	const UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
	if (!GI)
	{
		UE_LOG(LogTemp, Warning, TEXT("GI is null, returning WarriorPawnClass"));
		return WarriorPawnClass;
	}

	switch (GI->SelectedPlayerClass)
	{
	case EPlayerClassType::Warrior:
		UE_LOG(LogTemp, Warning, TEXT("Selected class: Warrior"));
		return WarriorPawnClass;

	case EPlayerClassType::Mage:
		UE_LOG(LogTemp, Warning, TEXT("Selected class: Mage"));
		return MagePawnClass;

	case EPlayerClassType::Archer:
		UE_LOG(LogTemp, Warning, TEXT("Selected class: Archer"));
		return ArcherPawnClass;
	}

	UE_LOG(LogTemp, Warning, TEXT("Fallback to WarriorPawnClass"));
	return WarriorPawnClass;
}
