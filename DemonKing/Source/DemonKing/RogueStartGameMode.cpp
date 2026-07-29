#include "RogueStartGameMode.h"
#include "GameFlow/MyGameInstance.h"

#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"

ARogueStartGameMode::ARogueStartGameMode()
{
}

void ARogueStartGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (!NewPlayer)
	{
		return;
	}

	PendingPlayerController = NewPlayer;

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

void ARogueStartGameMode::TrySpawnPendingPlayer()
{
	if (!PendingPlayerController)
	{
		GetWorldTimerManager().ClearTimer(SpawnRetryTimerHandle);
		return;
	}

	// 이미 Pawn이 있으면 종료
	if (PendingPlayerController->GetPawn())
	{
		GetWorldTimerManager().ClearTimer(SpawnRetryTimerHandle);
		return;
	}

	FTransform SpawnTransform;
	if (!FindGroundedSpawnTransform(SpawnTransform))
	{
		// 아직 바닥이 준비 안 됐으면 다음 틱에 다시 시도
		return;
	}

	// 바닥을 찾았을 때만 실제 스폰
	RestartPlayer(PendingPlayerController);

	// 스폰 성공 여부 확인
	if (PendingPlayerController->GetPawn())
	{
		GetWorldTimerManager().ClearTimer(SpawnRetryTimerHandle);
	}
}

AActor* ARogueStartGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return Super::ChoosePlayerStart_Implementation(Player);
	}

	for (TActorIterator<APlayerStart> It(World); It; ++It)
	{
		return *It;
	}

	return Super::ChoosePlayerStart_Implementation(Player);
}

bool ARogueStartGameMode::FindGroundedSpawnTransform(FTransform& OutTransform) const
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}

	AActor* StartActor = const_cast<ARogueStartGameMode*>(this)->ChoosePlayerStart_Implementation(nullptr);
	if (!StartActor)
	{
		return false;
	}

	const FVector BaseLocation = StartActor->GetActorLocation();
	const FVector TraceStart = BaseLocation + FVector(0.f, 0.f, TraceStartZOffset);
	const FVector TraceEnd = BaseLocation + FVector(0.f, 0.f, -TraceEndZOffset);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	const bool bHit = World->LineTraceSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		ECC_Visibility,
		QueryParams
	);

	if (!bHit)
	{
		return false;
	}

	const FVector FinalSpawnLocation = HitResult.Location + FVector(0.f, 0.f, SpawnZOffset);
	const FRotator FinalSpawnRotation = StartActor->GetActorRotation();

	OutTransform = FTransform(FinalSpawnRotation, FinalSpawnLocation);
	return true;
}

APawn* ARogueStartGameMode::SpawnDefaultPawnAtTransform_Implementation(
	AController* NewPlayer,
	const FTransform& SpawnTransform)
{
	UWorld* World = GetWorld();
	TSubclassOf<APawn> SelectedPawnClass = GetSelectedPawnClass();

	if (!World || !SelectedPawnClass)
	{
		return Super::SpawnDefaultPawnAtTransform_Implementation(NewPlayer, SpawnTransform);
	}

	// 여기서도 한 번 더 최종 보정
	FTransform FinalTransform;
	if (!FindGroundedSpawnTransform(FinalTransform))
	{
		// 바닥 못 찾으면 아예 스폰하지 않음
		return nullptr;
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
