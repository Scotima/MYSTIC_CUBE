// Fill out your copyright notice in the Description page of Project Settings.
#include "DemonKing/GameFlow/CStageGameMode.h"
#include "NavigationSystem.h"
#include "GameFramework/Pawn.h"
#include "DemonKing/ActorComponent/EnemyComponent/CEnemyStatComponent.h"
#include "Kismet/GameplayStatics.h"

void ACStageGameMode::GetPlayerInform() // 플레이어 위치 정보 얻어오기.
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
			continue;
		}

		PlayerPawns.Add(Player); // 플레이어들을 저장.
	}

}

void ACStageGameMode::PrepareForSpawnMonster()
{
	MonsterSpawnLocations.Reset();
	

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());

	if (!NavSystem)
	{
		return;
	}

	if (PlayerPawns.IsEmpty())
	{
		return;
	}
	
	for (const auto Player : PlayerPawns)
	{
		APawn* PlayerPawn = Player.Get();

		if (!IsValid(PlayerPawn))
		{
			continue;
		}

		const FVector PlayerLocation = PlayerPawn->GetActorLocation();

		for (int32 index = 0; index < MonsterCount; index++)
		{
			FNavLocation NavSpawnLocation;

			const bool bFoundLocation =
				NavSystem->GetRandomReachablePointInRadius(
					PlayerLocation, 1500.0f, NavSpawnLocation);


			if (bFoundLocation)
			{
				MonsterSpawnLocations.Add(NavSpawnLocation);
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

	bool bSpawnedMonster = false;

	for (const auto MonsterLocation : MonsterSpawnLocations)
	{
		const FTransform SpawnTransform(FRotator::ZeroRotator, MonsterLocation);

		APawn* SpawnedMonster = world->SpawnActorDeferred<APawn>(MonsterClass,
			SpawnTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding);

		if (!IsValid(SpawnedMonster))
		{
			continue;
		}
		UCEnemyStatComponent* StatComponent =
			SpawnedMonster->FindComponentByClass<UCEnemyStatComponent>();

		if (!IsValid(StatComponent))
		{
			SpawnedMonster->Destroy();
			continue;
		}
		
		StatComponent->SetMaxHp(MonsterInfo.Health);
		StatComponent->SetAttackPower(MonsterInfo.Damage);

		UGameplayStatics::FinishSpawningActor(SpawnedMonster, SpawnTransform);
	}


}



void ACStageGameMode::InputStageInformation()
{

	FMonster_Imformation MonsterInfo;
	MonsterInfo.Damage = 100;
	MonsterInfo.Health = 100;
	MonsterInfo.MonsterCount = 50;

	MonsterQueue.Enqueue(MonsterInfo); // Stage 1


}

void ACStageGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	GetPlayerInform();

}


