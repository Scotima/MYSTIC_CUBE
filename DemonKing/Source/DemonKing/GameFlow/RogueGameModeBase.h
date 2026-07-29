#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DemonKing/GameFlow/RogueRunTypes.h"
#include "RogueGameModeBase.generated.h"

USTRUCT(BlueprintType)
struct FStageMapList
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Run|Stages")
	TArray<FName> MapPaths;
};


UCLASS()
class DEMONKING_API ARogueGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	ARogueGameModeBase();

	//RunFlow (Server only)

	UFUNCTION(Exec)
	void StartRun();

	UFUNCTION(Exec)
	void AdvanceStage();

	UFUNCTION(Exec)
	void EndRun(ERogueRunEndReason Reason);

	UFUNCTION(Exec)
	void AdvanceMapWithinStage();





protected:
	virtual void StartPlay() override;

protected:

	//로비 /런 맵 경로 (나중에 변경 쉬움.)
	UPROPERTY(EditDefaultsOnly, Category = "Run|Maps")
	FString LobbyMapPath = TEXT("/Game/Maps/L_Lobby");

	//지금은 스테이지가 3개여도 맵은 L_Run 하나로 고정
	UPROPERTY(EditDefaultsOnly, Category = "Run|Maps")
	FString DefaultRunMapPath = TEXT("/Game/Maps/L_Showcase_01");

	//스테이지 ID 목록(이름 미정일 때 기본값.)
	UPROPERTY(EditDefaultsOnly, Category = "Run|Stages")
	TArray <FName> StageOrder;
	
	//StageId->MapPath 매핑(스테이지별 맵 생기면 여기만 채우면 됨.)
	UPROPERTY(EditDefaultsOnly, Category = "Run|Stages")
	TMap<FName, FStageMapList> StageToMaps;

	//같은 맵이 나올 수 있게 하는 옵션
	UPROPERTY(EditDefaultsOnly, Category = "Run|Stages")
	bool bAllowSameMapRepeat = true; //true면 같은 맵도 다시 선택 될 수 있음.

	UPROPERTY(EditDefaultsOnly, Category = "Run|Stages", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float KeepSameMapChance = 0.35f; // 같은 맵 유지 확률 (0~1)

private:
	class ARogueGameState* GetRogueGS() const;

	FName GetNextStageId(const FName& CurrentStageId) const;

	//스테이지 + (스테이지 내부 인덱스)로 트래블
	void ServerTravelToStage(const FName& StageId, int32 StageMapIndex);

	bool EnsureServerAuth(const TCHAR* FuncName) const;

	int32 GetStageOrderIndexSafe(const FName& StageId) const;
	

	class URogueSaveSubsystem* GetSaveSS() const;



private:
	int32 StageStep_Runtime = 0;


	
};
