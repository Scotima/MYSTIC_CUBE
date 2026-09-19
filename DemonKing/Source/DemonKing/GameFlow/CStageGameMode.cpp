// Fill out your copyright notice in the Description page of Project Settings.
#include "DemonKing/GameFlow/CStageGameMode.h"
#include "NavigationSystem.h"
#include "GameFramework/Pawn.h"
#include "DemonKing/ActorComponent/EnemyComponent/CEnemyStatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DemonKing/GameFlow/MyGameInstance.h"
#include "Components/CapsuleComponent.h"
#include "CollisionShape.h"

void ACStageGameMode::GetPlayerInform() 
{
	UWorld* World = GetWorld();

	if (!World || !HasAuthority())
	{
		return;
	}

	for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; It++)
	{
		APlayerController* PlayerController = It->Get();	

		if (!IsValid(PlayerController))
		{
			continue;
		}

		APawn* Player = PlayerController->GetPawn();

		if (!IsValid(Player))
		{
			UE_LOG(LogTemp, Warning, TEXT("[ACStageGameMode] GetPlayerInform() !IsValid(Player)"));
			continue;
		}

		PlayerPawns.Add(Player); 
	}

}

void ACStageGameMode::PrepareForSpawnMonster()
{
	if (!HasAuthority())
	{
		return;
	}

	RandomMonsterInform.Reset();
	MonsterAliveCount = 0;

	UWorld* World = GetWorld();

	if (!World)
	{
		return;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());

	if (!NavSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ACStageGameMode] PrepareForSpawnMonster() !NavSystem"));
		return;
	}

	if (PlayerPawns.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("[ACStageGameMode]  PrepareForSpawnMonster() PlayerPawns.IsEmpty()"));
		return;
	}
	
	if (MonsterArray.IsEmpty())
	{
		return;
	}

	FStageMonsterInformation MonsterInfo;

	if (!MonsterQueue.Peek(MonsterInfo))
	{
		UE_LOG(LogTemp, Warning, TEXT("!MonsterQueue.Peek(MonsterInfo)"));
		return;
	}

	if (MonsterInfo.MonsterCount <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("MonsterCount <= 0"));
		return;
	}

	TArray<AActor*> SpawnPointActors;

	UGameplayStatics::GetAllActorsWithTag(World, FName(TEXT("MonsterSpawn")), SpawnPointActors);

	if (SpawnPointActors.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnPointActors.IsEmpty"));
		return;
	}

	for (int32 Index = SpawnPointActors.Num() - 1; Index > 0; --Index)
	{
		const int32 SwapIndex = FMath::RandRange(0, Index);
		SpawnPointActors.Swap(Index, SwapIndex);
	}

	int32 ValidPlayerCount = 0;

	for (TWeakObjectPtr<APawn>& Player : PlayerPawns)
	{
		if (Player.IsValid())
		{
			ValidPlayerCount++;
		}
	}

	if (ValidPlayerCount <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("ValidPlayerCount <= 0"));
		return;
	}

	const int32 TotalMonsterCount = MonsterInfo.MonsterCount * ValidPlayerCount;

	constexpr float SpawnRadius = 500.0f;
	constexpr float MininumPlayerDistance = 1200.0f;
	constexpr int32 MaxAttempt = 10;

	for (int32 MonsterIndex = 0; MonsterIndex < TotalMonsterCount; ++MonsterIndex)
	{
		AActor* SpawnPoint = SpawnPointActors[MonsterIndex % SpawnPointActors.Num()]; // 스폰포인트를 순환해서 한쪽으로 몰리지 않게 한다.

		if (!IsValid(SpawnPoint))
		{
			UE_LOG(LogTemp, Warning, TEXT("!IsValid(SpawnPoint)"));
			return;
		}

		FNavLocation ProjectedSpawnCenter;

		const FVector ProjectionExtent(
			500.0f, 500.0f, 1000.0f);

		if (!NavSystem->ProjectPointToNavigation(
			SpawnPoint->GetActorLocation(), ProjectedSpawnCenter, ProjectionExtent))
		{
			UE_LOG(LogTemp, Warning, TEXT("OutSide NaveMesh = %s"), * GetNameSafe(SpawnPoint));
			continue;
		}


		TSubclassOf<APawn> SelectedMonsterClass = nullptr;

		const int32 RandomClassIndex = FMath::RandRange(0, MonsterArray.Num() - 1);

		for (int32 Offset = 0; Offset < MonsterArray.Num(); Offset++)
		{
			const int32 ClassIndex = (RandomClassIndex + Offset) % MonsterArray.Num();

			if (MonsterArray[ClassIndex])
			{
				SelectedMonsterClass = MonsterArray[ClassIndex];
				break;
			}
		}

		if (!SelectedMonsterClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("!SelectedMonster"));
			return;
		}

		bool bPreparedLocation = false;

		for (int32 Attempt = 0; Attempt < MaxAttempt; Attempt++)
		{
			FNavLocation NavSpawnLocation;

			const bool bFoundRandomLocation = NavSystem->GetRandomReachablePointInRadius(
				ProjectedSpawnCenter.Location, SpawnRadius, NavSpawnLocation);

			if (!bFoundRandomLocation)
			{
				continue;
			}

			bool bTooCloseToPlayer = false;

			for (const TWeakObjectPtr<APawn>& Player : PlayerPawns)
			{
				const APawn* PlayerPawn = Player.Get();

				if (!PlayerPawn)
				{
					continue;
				}

				const float DistanceSquared = FVector::DistSquared(NavSpawnLocation.Location, PlayerPawn->GetActorLocation());

				if (DistanceSquared < FMath::Square(MininumPlayerDistance))
				{
					bTooCloseToPlayer = true;
					break;
				}

			}

			if (bTooCloseToPlayer)
			{
				continue;
			}

			FVector ClearSpawnLocation;

			if (!IsMonsterSpawnLocationClear(NavSpawnLocation.Location, ClearSpawnLocation, SelectedMonsterClass))
			{
				continue;
			}

			FMonster_Class_LocationInform PreparedMonster;
			PreparedMonster.MonsterCharacterArray = SelectedMonsterClass;
			PreparedMonster.MonsterLocationArray = ClearSpawnLocation;

			RandomMonsterInform.Add(PreparedMonster);
			++MonsterAliveCount;

			bPreparedLocation = true;
			break;


		}

		if (!bPreparedLocation)
		{

			UE_LOG(
				LogTemp,
				Warning,
				TEXT("[ACStageGameMode] Failed preparing spawn near: %s"),
				*GetNameSafe(SpawnPoint)
			);
		}


	}


	UE_LOG(
		LogTemp,
		Warning,
		TEXT("[ACStageGameMode] Prepared monsters: %d / %d"),
		RandomMonsterInform.Num(),
		TotalMonsterCount
	);
	
}

void ACStageGameMode::SpawnMonster()
{

	if (!HasAuthority())
	{
		return;
	}

	if (RandomMonsterInform.IsEmpty())
	{
		return;
	}

	UWorld* world = GetWorld();

	if (!world)
	{
		return;
	}

	FStageMonsterInformation MonsterInfo;
	if (!MonsterQueue.Dequeue(MonsterInfo))
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[ACStageGameMode] SpawnMonster() ddddddd"));
	for (const auto& PrepareMonster : RandomMonsterInform)
	{
		
		const bool bSpawnedMonster = TrySpawnSingleMonster(PrepareMonster.MonsterLocationArray, MonsterInfo,
		PrepareMonster.MonsterCharacterArray);

		if (!bSpawnedMonster)
		{
			UE_LOG(LogTemp, Warning, TEXT("[ACStageGameMode] SpawnMonster !bSpawnedMonster"));
			continue;
		}

		
		
		
	}

	


}



void ACStageGameMode::InputStageInformation()
{
	if (!IsValid(StageMonsterDataTable))
	{
		UE_LOG(LogTemp, Warning, TEXT("!StageMonsterDataTable"));
		return;
	}

	TArray<FStageMonsterInformation*> StageRows;

	StageMonsterDataTable->GetAllRows<FStageMonsterInformation>(TEXT("InputStageInformation"), StageRows);

	StageRows.Sort([](const FStageMonsterInformation& A, const FStageMonsterInformation& B) {
		return A.StageIndex < B.StageIndex; });

	MonsterQueue.Empty();

	for (const FStageMonsterInformation* StageRow : StageRows)
	{
		if (!StageRow)
		{
			UE_LOG(LogTemp, Warning, TEXT("!StageRow"));
			bIsReadySpawnMonster = false;
			return;
		}

		MonsterQueue.Enqueue(*StageRow);
	}

}

bool ACStageGameMode::All_Expected_Player_Spawned()
{
	if (ExpectedPlayerNum <= 0)
	{
		return false;
	}

	if (!PendingPlayerController.IsEmpty())
	{
		return false;
	}

	const UWorld* world = GetWorld();

	if (!world)
	{
		return false;
	}

	int32 SpawnedPlayerNum = 0;

	for (FConstPlayerControllerIterator It = world->GetPlayerControllerIterator();
		It; ++It)
	{
		APlayerController* pc = It->Get();

		if (IsValid(pc) && IsValid(pc->GetPawn()))
		{
			++SpawnedPlayerNum;
		}

		else
		{
			continue;
		}
	}
	return SpawnedPlayerNum >= ExpectedPlayerNum;



}

bool ACStageGameMode::TrySpawnSingleMonster(const FVector& PreparedSpawnLocation, const FStageMonsterInformation& MonsterInfo, TSubclassOf<APawn> Monster)
{
	UWorld* world = GetWorld();

	if (!world)
	{
		return false;
	}

	if (!HasAuthority())
	{
		return false;
	}

	if (!IsValid(Monster))
	{
		return false;
	}

	

	MonsterInfo;

	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
	APawn* SpawnedMonster = world->SpawnActor<APawn>(Monster, PreparedSpawnLocation, FRotator::ZeroRotator,
		SpawnParams);

	if (!IsValid(SpawnedMonster))
	{
		UE_LOG(LogTemp, Warning, TEXT("[ACStageMode]::TrySpawnSingleMonster : !IsValid(SpawnedMonster)"));
		--MonsterAliveCount;
		return false;
	}

	UE_LOG(LogTemp, Warning,
		TEXT("[TrySpawnSingleMonster] Spawn success: %s, Location=%s"),
		*GetNameSafe(SpawnedMonster),
		*SpawnedMonster->GetActorLocation().ToString());

	UCEnemyStatComponent* StatComponent = SpawnedMonster->FindComponentByClass<UCEnemyStatComponent>();
	if (!IsValid(StatComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("[ACStageMode]::TrySpawnSingleMonster: !IsValid(StatComponent)"));
		SpawnedMonster->Destroy();
		--MonsterAliveCount;
		return false;
	}

	StatComponent->SetMaxHp(MonsterInfo.Health);
	StatComponent->SetAttackPower(MonsterInfo.Damage);

	StatComponent->OnEnemyDied.AddUObject(this, &ACStageGameMode::HandleEnemyDied);


	return true;

}

bool ACStageGameMode::IsMonsterSpawnLocationClear(const FVector& NavFloorLocation, FVector& OutSpawnLocation, TSubclassOf<APawn> Monster) const
{
	UWorld* world = GetWorld();

	if (!world || !Monster)
	{
		return false;
	}

	const APawn* MonsterCDO = Monster->GetDefaultObject<APawn>();

	if (!IsValid(MonsterCDO))
	{
		UE_LOG(LogTemp, Warning, TEXT("[ACStageGameMode] IsMonsterSpawnLocationClear !IsValid MonsterCDO"));
		return false;
	}


	const UCapsuleComponent* Capsule = MonsterCDO->FindComponentByClass<UCapsuleComponent>();

	if (!IsValid(Capsule))
	{
		UE_LOG(LogTemp, Warning, TEXT("[ACStageGameMode] IsMonsterSpawnLocationClear !IsValid Capsule"));
		return false;
	}


	constexpr float Clearance = 10.0f;

	const float TestRadius = Capsule->GetScaledCapsuleRadius() + Clearance;
	const float TestHalfHeight = Capsule->GetScaledCapsuleHalfHeight() + Clearance;

	OutSpawnLocation = NavFloorLocation + FVector::UpVector * TestHalfHeight;

	const FCollisionShape TestShape = FCollisionShape::MakeCapsule(TestRadius, TestHalfHeight);

	const bool bHasBlockOverlap = world->OverlapBlockingTestByProfile(OutSpawnLocation,
		FQuat::Identity, Capsule->GetCollisionProfileName(), TestShape, FCollisionQueryParams::DefaultQueryParam);

	return !bHasBlockOverlap;

}

void ACStageGameMode::StartGameOverlap()
{

	if (bEncounterStarted || !bIsReadySpawnMonster)
	{
		return;
	}

	bEncounterStarted = true;

	PrepareForSpawnMonster();

	if (RandomMonsterInform.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("[ACStageGameMode::OnSpawnQueueDrained] MonsterSpawnLocations.IsEmpty()"));
		bEncounterStarted = false;
		return;
	}


	
	
	

	SpawnDelayTimer(0.5f);

	StageState = EndStage::Playing;
	
}

void ACStageGameMode::HandleEnemyDied()
{
	MonsterAliveCount = FMath::Max(MonsterAliveCount - 1, 0);

	if (MonsterAliveCount > 0)
	{
		return;
	}

	//Stage 종료
	StageState = EndStage::Cleared;

	if (StageState == EndStage::Cleared)
	{
		PrepareForSpawnMonster();

		if (RandomMonsterInform.IsEmpty())
		{
			UE_LOG(LogTemp, Warning, TEXT("[ACStageGameMode::OnSpawnQueueDrained] MonsterSpawnLocations.IsEmpty()"));
			bEncounterStarted = false;
			return;
		}

		SpawnDelayTimer(0.5f);
		StageState = EndStage::Playing;
	}
}

void ACStageGameMode::SpawnDelayTimer(float time)
{
	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("[ACStageGameMode] :: SpawnDelayTimer !HasAuthority"));
		return;
	}

	if (GetWorldTimerManager().IsTimerActive(SpawnDelay))
	{
		UE_LOG(LogTemp, Warning, TEXT("[ACStageGameMode] :: SpawnDelayTimer !GetWorldTimerManager"));
		return;
	}

	GetWorldTimerManager().SetTimer(SpawnDelay, this, &ACStageGameMode::SpawnMonster, FMath::Max(time, 0.0f), false);


}

void ACStageGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

}

void ACStageGameMode::StartPlay()
{
	Super::StartPlay();

	UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());

	if (!GI)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ACStageGameMode] !GI"));
		return;
	}

	ExpectedPlayerNum = GI->GetExpectedStagePlayerCount();

	UE_LOG(LogTemp, Warning, TEXT("[ACStageGameMode] Player Num = %d"), ExpectedPlayerNum);


}

void ACStageGameMode::OnSpawnQueueDrained()
{
	UE_LOG(LogTemp, Warning, TEXT("[ACStageGameMode] OnSpawnQueueDrained()"));

	if (bIsReadySpawnMonster)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ACStageGameMode]OnSpawnQueueDrained() bIsReadyMonster"));
		return;
	}

	if (!All_Expected_Player_Spawned())
	{
		UE_LOG(LogTemp, Warning, TEXT("[ACStageGameMode] OnSpawnQueueDrained() !All_Expected_Player_Spawned"));
		return;
	}
	
	if (MonsterArray.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("[ACStageGameMode] OnSpawnQueueDrained() !MonsterClass"));
		return;
	}

	PlayerPawns.Reset();

	GetPlayerInform();

	InputStageInformation();


	bIsReadySpawnMonster = true;

}


