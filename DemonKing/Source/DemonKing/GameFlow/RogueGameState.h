#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "DemonKing/GameFlow/RogueRunTypes.h"
#include "RogueGameState.generated.h"




DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRunActiveChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStageIdChanged, FName, NesStageId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndReasonChanged, ERogueRunEndReason, NewReason);

//스테이지 내부 맵 인덱스 변경 이벤트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStageMapIndexChanged, int32, NewIndex);

//시드 기반 랜덤을 위한 이벤트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRunSeedChanged, int32, NewSeed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStageSeedChanged, int32, NewSeed);

UCLASS()
class DEMONKING_API ARogueGameState : public AGameStateBase
{
	GENERATED_BODY()



public:

	ARogueGameState();
	//런 활성 / 비활성화.
	UFUNCTION(BlueprintCallable, Category = "Run")
	void SetRunActive(bool bInRunActive);

	//스테이지 ID 세팅 (서버 전용.)
	UFUNCTION(BlueprintCallable, Category = "Run")
	void SetStageId(FName NewStageId);

	// 종료 이유 세팅(서버 전용)
	UFUNCTION(BlueprintCallable, Category = "Run")
	void SetEndReason(ERogueRunEndReason NewReason);

	UFUNCTION(BlueprintCallable, Category = "Run")
	void SetStageMapIndex(int32 NewIndex);

	//런 전체 시드 세팅(서버전용)
	UFUNCTION(BlueprintCallable, Category = "Run")
	void SetRunSeed(int32 NewSeed);

	//스테이지 콘텐츠 시드(StageSeed) 세팅
	UFUNCTION(BlueprintCallable, Category = "Run")
	void SetStageSeed(int32 NewSeed);

	// 런 관련 상태 초기화 하고 싶을 때
	UFUNCTION(BlueprintCallable, Category = "Run")
	void ResetRunState();




	UFUNCTION(BlueprintPure, Category = "RUN")
	bool GetRunActive() const { return bRunActive; }

	UFUNCTION(BlueprintPure, Category = "Run")
	FName GetStageId() const { return CurrentStageId; }

	UFUNCTION(BlueprintPure, Category = "Run")
	ERogueRunEndReason GetEndReason() const { return EndReason;}

	UFUNCTION(BlueprintPure, Category = "Run")
	int32 GetStageMapIndex() const { return StageMapIndex; }

	//런 전체 시드(맵//콘텐츠 랜덤의 기준)
	UFUNCTION(BlueprintPure, Category = "Run")
	int32 GetRunSeed() const { return RunSeed;}

	//스테이지 콘텐츠 시드 (몬스터/보상 배치 등)
	UFUNCTION(BlueprintPure, Category = "Run")
	int32 GetStageSeed() const { return StageSeed; }

protected:
	
	UFUNCTION()
	void OnRep_RunActive();

	UFUNCTION()
	void OnRep_CurrentStageId();

	UFUNCTION()
	void OnRep_EndReason();

	UFUNCTION()
	void OnRep_StageMapIndex();

	UFUNCTION()
	void OnRep_RunSeed();

	UFUNCTION()
	void OnRep_StageSeed();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:

	void NotifyRunActiveChanged_Server();

	void NotifyStageIdChanged_Server();

	void NotifyEndReasonChanged_Server();

	//로컬 서버 이벤트용
	void NotifyStageMapIndexChanged_Server();

	void NotifyRunSeedChanged_Server();

	void NotifyStageSeedChanged_Server();
	             



public:

	// 런이 진행 중인지.

	UPROPERTY(ReplicatedUsing = OnRep_RunActive, BlueprintReadOnly, Category = "Run")
	bool bRunActive = false;

	//현재 스테이지.
	UPROPERTY(ReplicatedUsing = OnRep_CurrentStageId, BlueprintReadOnly, Category = "Run")
	FName CurrentStageId = NAME_None;


	//런 종료 이유.
	UPROPERTY(ReplicatedUsing = OnRep_EndReason, BlueprintReadOnly, Category = "Run")
	ERogueRunEndReason EndReason = ERogueRunEndReason::None;

	UPROPERTY(ReplicatedUsing = OnRep_StageMapIndex, BlueprintReadOnly, Category = "Run")
	int32 StageMapIndex = 0;

	UPROPERTY(ReplicatedUsing = OnRep_RunSeed, BlueprintReadOnly, Category = "Run")
	int32 RunSeed = 0;

	UPROPERTY(ReplicatedUsing = OnRep_StageSeed, BlueprintReadOnly, Category = "Run")
	int32 StageSeed = 0;


public:

	// UI/BP Event(선택)
	UPROPERTY(BlueprintAssignable, Category = "Run")
	FOnRunActiveChanged OnRunActiveChanged;

	UPROPERTY(BlueprintAssignable, Category = "Run")
	FOnStageIdChanged OnStageIdChanged;

	UPROPERTY(BlueprintAssignable, Category = "Run")
	FOnEndReasonChanged OnEndReasonChanged;

	UPROPERTY(BlueprintAssignable, Category = "Run")
	FOnStageMapIndexChanged OnStageMapIndexChanged;

	UPROPERTY(BlueprintAssignable, Category = "Run")
	FOnRunSeedChanged OnRunSeedChanged;

	UPROPERTY(BlueprintAssignable, Category = "Run")
	FOnStageSeedChanged OnStageSeedChanged;
	
};
