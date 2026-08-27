#pragma once


#include "CoreMinimal.h"
#include "DemonKing/RogueStartGameMode.h"
#include "Containers/Queue.h"
#include "CStageGameMode.generated.h"

USTRUCT(BlueprintType)
struct  FMonster_Imformation
{
	GENERATED_BODY()

	float Damage;
	float Health;
	int32 MonsterCount;

};

UENUM(BlueprintType)

enum class EndStage : uint8
{
	None,
	Playing,
	Cleared
};

USTRUCT(BlueprintType)
struct FMonster_Class_LocationInform
{
	GENERATED_BODY()
	TSubclassOf<APawn> MonsterCharacterArray;
	FVector MonsterLocationArray;
};

UCLASS()
class DEMONKING_API ACStageGameMode : public ARogueStartGameMode
{
	GENERATED_BODY()
	


public:
	void GetPlayerInform();
	void PrepareForSpawnMonster();
	void SpawnMonster();

	void InputStageInformation();

	bool All_Expected_Player_Spawned();

	bool TrySpawnSingleMonster(const FVector& PreparedSpawnLocation, const FMonster_Imformation& MonsterInfo, TSubclassOf<APawn> Monster);

	bool IsMonsterSpawnLocationClear(const FVector& NavFloorLocation, FVector& OutSpawnLocation, TSubclassOf<APawn> Monster) const;


	UFUNCTION(BlueprintCallable)
	void StartGameOverlap();


	void HandleEnemyDied();

	void SpawnDelayTimer(float time);

protected:
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	virtual void StartPlay() override;

	virtual void OnSpawnQueueDrained() override;
	



protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Monster")
	TArray<TSubclassOf<APawn>> MonsterArray;

	/*UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Monster")
	TSubclassOf<APawn> MonsterClass;*/

	UPROPERTY(EditDefaultsOnly, Category = "Monster")
	int32 MonsterCount = 3;
private:
	TArray<TWeakObjectPtr<APawn>> PlayerPawns; // 접속한 플레이어 정보 저장.
	FRandomStream SpawnRandomStream;

	TArray<FVector> MonsterSpawnLocations;
	TArray<FMonster_Class_LocationInform> RandomMonsterInform;
	TQueue<FMonster_Imformation> MonsterQueue;

	int32 ExpectedPlayerNum = 0;

	bool bIsReadySpawnMonster = false;

	bool bEncounterStarted = false;

	int32 MonsterAliveCount = 0;

	EndStage StageState = EndStage::None;

	FTimerHandle SpawnDelay;
	

	
};

//시드 즉 난수를 발생시켜서 몬스터의 위치를 랜덤하게 소환하는 역할.
//자료구조 큐
//큐 선입 선출을 원칙으로
//큐를 이용해서 미니 스테이지 몬스터 체력이나 방어력 이런것들을 점점 강하게 하기 위한 것들을 큐에 저장
//큐에서 하나씩 꺼내와서 한다
//큐 내용이 비어졌다 라고 한다면 클리어.
//다음 단계로.