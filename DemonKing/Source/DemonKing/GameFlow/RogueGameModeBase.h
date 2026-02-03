#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DemonKing/GameFlow/RogueRunTypes.h"
#include "RogueGameModeBase.generated.h"



UCLASS()
class DEMONKING_API ARogueGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	ARogueGameModeBase();

	UFUNCTION(Exec)
	void StartRun();

	UFUNCTION(Exec)
	void AdvanceStage();

	UFUNCTION(Exec)
	void EndRun(ERogueRunEndReason Reason);

protected:
	virtual void StartPlay() override;

protected:

	//로비 /런 맵 경로 (나중에 변경 쉬움.)
	UPROPERTY(EditDefaultsOnly, Category = "Run|Maps")
	FString LobbyMapPath = TEXT("/Game/Maps/L_Lobby");

	//지금은 스테이지가 3개여도 맵은 L_Run 하나로 고정
	UPROPERTY(EditDefaultsOnly, Category = "Run|Maps")
	FString DefaultRunMapPath = TEXT("/Game/Maps/L_Run");

	//스테이지 ID 목록(이름 미정일 때 기본값.)
	UPROPERTY(EditDefaultsOnly, Category = "Run|Stages")
	TArray <FName> StageOrder;
	
	//StageId->MapPath 매핑(스테이지별 맵 생기면 여기만 채우면 됨.)
	UPROPERTY(EditDefaultsOnly, Category = "Run|Stages")
	TMap<FName, FString> StageToMapPath;

private:
	class ARogueGameState* GetRogueGS() const;

	FName GetNextStageId(const FName& CurrentStageId) const;

	void ServerTravelToStage(const FName& StageId);

	bool EnsureServerAuth(const TCHAR* FuncName) const;


	
};
