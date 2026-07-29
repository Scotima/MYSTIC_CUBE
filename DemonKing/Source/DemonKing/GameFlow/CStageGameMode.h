#pragma once


#include "CoreMinimal.h"
#include "DemonKing/GameFlow/RogueGameModeBase.h"
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


UCLASS()
class DEMONKING_API ACStageGameMode : public ARogueGameModeBase
{
	GENERATED_BODY()
	


public:
	void GetPlayerInform();
	void PrepareForSpawnMonster();
	void SpawnMonster();

	void InputStageInformation();

protected:
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	



protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Monster")
	TSubclassOf<APawn> MonsterClass;

	UPROPERTY(EditDefaultsOnly, Category = "Monster")
	int32 MonsterCount = 3;
private:
	TArray<TWeakObjectPtr<APawn>> PlayerPawns; // 접속한 플레이어 정보 저장.
	FRandomStream SpawnRandomStream;

	TArray<FVector> MonsterSpawnLocations;
	TQueue<FMonster_Imformation> MonsterQueue;
	

	
};

//시드 즉 난수를 발생시켜서 몬스터의 위치를 랜덤하게 소환하는 역할.
//자료구조 큐
//큐 선입 선출을 원칙으로
//큐를 이용해서 미니 스테이지 몬스터 체력이나 방어력 이런것들을 점점 강하게 하기 위한 것들을 큐에 저장
//큐에서 하나씩 꺼내와서 한다
//큐 내용이 비어졌다 라고 한다면 클리어.
//다음 단계로.