// Fill out your copyright notice in the Description page of Project Settings.
#include "DemonKing/GameFlow/CStageGameMode.h"
#include "NavigationSystem.h"
#include "GameFramework/Pawn.h"
#include "DemonKing/ActorComponent/EnemyComponent/CEnemyStatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DemonKing/GameFlow/MyGameInstance.h"
#include "Components/CapsuleComponent.h"
#include "CollisionShape.h"

void ACStageGameMode::GetPlayerInform() // �÷��̾� ��ġ ���� ������.
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

		PlayerPawns.Add(Player); // �÷��̾���� ����.
	}

}

void ACStageGameMode::PrepareForSpawnMonster()
{
	MonsterSpawnLocations.Reset();
	

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
	
	for (const auto& Player : PlayerPawns)
	{
		APawn* PlayerPawn = Player.Get();

		if (!IsValid(PlayerPawn))
		{
			UE_LOG(LogTemp, Warning, TEXT("[ACStageGameMode]  PrepareForSpawnMonster() !IsValid(PlayerPawn)"));
			continue;
		}

		const FVector PlayerLocation = PlayerPawn->GetActorLocation();
		const FVector ProjectionExtent(500.0f, 500.0f, 1500.0f);

		FNavLocation ProjectedPlayerLocation;


		if (!NavSystem->ProjectPointToNavigation(PlayerLocation,
			ProjectedPlayerLocation, ProjectionExtent))
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed Finding NavMesh Where Player Under. Player = %s"),
				*PlayerLocation.ToString());
			continue;
		}

		FMonster_Imformation MonsterInfo;
		if (!MonsterQueue.Peek(MonsterInfo))
		{
			UE_LOG(LogTemp, Warning,
				TEXT("[ACStageGameMode] ::PrepareForSpawnMonster "
					"MonsterQueue is empty"));

			return;
		}

		int32 Monster_Count = MonsterInfo.MonsterCount;

		if (Monster_Count <= 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("[ACStageGameMode] :: PrepareForSpawnMonster"
				" Monster Count Under Zero"));
			return;
		}

		for (int32 index = 0; index < Monster_Count; index++)
		{
			FNavLocation NavSpawnLocation;

			
			int32 MaxAttempt = 10;
			bool bPreparedLocation = false;

			for (int32 Attempt = 0; Attempt < MaxAttempt; Attempt++)
			{
				const bool bFoundLocation =
					NavSystem->GetRandomReachablePointInRadius(
						ProjectedPlayerLocation.Location, 1500.0f, NavSpawnLocation);


				if (!bFoundLocation)
				{
					UE_LOG(LogTemp, Warning, TEXT("[ACStageGameMode]::PrepareForSpawnMonster !bFoundLocation"));
					continue;
				}

				FVector ClearSpawnLocation;

				if (!IsMonsterSpawnLocationClear(NavSpawnLocation.Location, ClearSpawnLocation))

				{
					continue;
				}

				
				MonsterSpawnLocations.Add(ClearSpawnLocation);
				MonsterAliveCount++;
				bPreparedLocation = true;
				break;
			}

			if (!bPreparedLocation)
			{
				UE_LOG(LogTemp, Warning, TEXT("[ACStageGameMode]::PrepareForSpawnMonster !bPreparedLocation"));
			}
			

			
		}




	}
	
}

void ACStageGameMode::SpawnMonster()
{

	if (!MonsterClass || !HasAuthority())
	{
		return;
	}

	if (MonsterSpawnLocations.IsEmpty())
	{
		return;
	}

	UWorld* world = GetWorld();

	if (!world)
	{
		return;
	}

	FMonster_Imformation MonsterInfo;
	if (!MonsterQueue.Dequeue(MonsterInfo))
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[ACStageGameMode] SpawnMonster() ddddddd"));
	for (const auto& MonsterLocation : MonsterSpawnLocations)
	{

		const bool bSpawnedMonster = TrySpawnSingleMonster(MonsterLocation, MonsterInfo);

		if (!bSpawnedMonster)
		{
			UE_LOG(LogTemp, Warning, TEXT("[ACStageGameMode] SpawnMonster !bSpawnedMonster"));
			continue;
		}

		
		
	}

	


}



void ACStageGameMode::InputStageInformation()
{

	FMonster_Imformation MonsterInfo;
	MonsterInfo.Damage = 100;
	MonsterInfo.Health = 100;
	MonsterInfo.MonsterCount = 3;

	MonsterQueue.Enqueue(MonsterInfo); // Stage 1


	MonsterInfo.Damage = 100;
	MonsterInfo.Health = 100;
	MonsterInfo.MonsterCount = 5;

	MonsterQueue.Enqueue(MonsterInfo);



	MonsterInfo.Damage = 100;
	MonsterInfo.Health = 100;
	MonsterInfo.MonsterCount = 10;

	MonsterQueue.Enqueue(MonsterInfo);


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

bool ACStageGameMode::TrySpawnSingleMonster(const FVector& PreparedSpawnLocation, const FMonster_Imformation& MonsterInfo)
{
	UWorld* world = GetWorld();

	if (!world)
	{
		return false;
	}

	if (!MonsterClass || !HasAuthority())
	{
		return false;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
	APawn* SpawnedMonster = world->SpawnActor<APawn>(MonsterClass, PreparedSpawnLocation, FRotator::ZeroRotator,
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

bool ACStageGameMode::IsMonsterSpawnLocationClear(const FVector& NavFloorLocation, FVector& OutSpawnLocation) const
{
	UWorld* world = GetWorld();

	if (!world || !MonsterClass)
	{
		return false;
	}

	const APawn* MonsterCDO = MonsterClass->GetDefaultObject<APawn>();

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

	if (MonsterSpawnLocations.IsEmpty())
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

		if (MonsterSpawnLocations.IsEmpty())
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
	
	if (!MonsterClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ACStageGameMode] OnSpawnQueueDrained() !MonsterClass"));
		return;
	}

	PlayerPawns.Reset();

	GetPlayerInform();

	InputStageInformation();


	bIsReadySpawnMonster = true;

}


