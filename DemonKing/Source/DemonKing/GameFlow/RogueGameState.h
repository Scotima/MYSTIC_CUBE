#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RogueGameState.generated.h"


// 런 종료 이유
UENUM(BlueprintType)
enum class ERogueRunEndReason : uint8
{
	None UMETA(DisplayName = "None"),
	Clear UMETA(DisplayName = "Clear"),
	PartyWipe UMETA(DisplayName = "PartyWipe"),
	Quit UMETA(DisplayName = "Quit"),
	Disconnect UMETA(DisplayName = "Disconnect"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRunActiveChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStageIdChanged, FName, NesStageId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndReasonChanged, ERogueRunEndReason, NewReason);

UCLASS()
class DEMONKING_API ARogueGameState : public AGameStateBase
{
	GENERATED_BODY()



public:

	ARogueGameState();
	//런 활성 / 비활성화.
	UFUNCTION(BlueprintCallable, Category = "Run")
	void SetRunActivate(bool bInRunActive);

	//스테이지 ID 세팅 (서버 전용.)
	UFUNCTION(BlueprintCallable, Category = "Run")
	void SetStageId(FName NewStageId);

	// 종료 이유 세팅(서버 전용)
	UFUNCTION(BlueprintCallable, Category = "Run")
	void SetEndReason(ERogueRunEndReason NewReason);

	// 런 관련 상태 초기화 하고 싶을 때
	UFUNCTION(BlueprintCallable, Category = "Run")
	void ResetRunState();

protected:
	
	UFUNCTION()
	void OnRep_RunActive();

	UFUNCTION()
	void OnRep_CurrentStageId();

	UFUNCTION()
	void OnRep_EndReason();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:

	void NotifyRunActiveChanged_Server();

	void NotifyStageIdChanged_Server();

	void NotifyEndReasonChanged_Server();
	             



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


public:

	// UI/BP Event(선택)
	UPROPERTY(BlueprintAssignable, Category = "Run")
	FOnRunActiveChanged OnRunActiveChanged;

	UPROPERTY(BlueprintAssignable, Category = "Run")
	FOnStageIdChanged OnStageIdChanged;

	UPROPERTY(BlueprintAssignable, Category = "Run")
	FOnEndReasonChanged OnEndReasonChanged;

	
};
