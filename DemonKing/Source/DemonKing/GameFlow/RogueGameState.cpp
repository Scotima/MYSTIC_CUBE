#include "DemonKing/GameFlow/RogueGameState.h"
#include "Net/UnrealNetwork.h"  
ARogueGameState::ARogueGameState()
{
	bReplicates = true;
}

void ARogueGameState::SetRunActivate(bool bInRunActive)
{
	if (!HasAuthority())
	{
		return;
	}

	if (bRunActive == bInRunActive)
	{
		return; // 중복 방지.
	}

	bRunActive = bInRunActive;

	//서버도 즉시 반응
	NotifyRunActiveChanged_Server();
}

void ARogueGameState::SetStageId(FName NewStageId)
{
	//서버 전용 가드
	if (!HasAuthority())
	{
		return;
	}

	if (CurrentStageId == NewStageId)
	{
		return; // 중복 세팅 방지
	}

	CurrentStageId = NewStageId;

	NotifyStageIdChanged_Server();
}

void ARogueGameState::SetEndReason(ERogueRunEndReason NewReason)
{
	if (!HasAuthority())
	{
		return;
	}

	if (EndReason == NewReason)
	{
		return; // [추가] 중복 세팅 방지
	}

	EndReason = NewReason;

	// [추가] 서버도 즉시 반응(선택)
	NotifyEndReasonChanged_Server();
}

void ARogueGameState::ResetRunState()
{
	if (!HasAuthority())
	{
		return;
	}

	bRunActive = false;
	CurrentStageId = NAME_None;
	EndReason = ERogueRunEndReason::None;

	NotifyRunActiveChanged_Server();
	NotifyStageIdChanged_Server();
	NotifyEndReasonChanged_Server();
}

void ARogueGameState::OnRep_RunActive()
{
	//클라에서 값 변경 시 UI 반응.
	OnRunActiveChanged.Broadcast();
}

void ARogueGameState::OnRep_CurrentStageId()
{
	//클라에서 값 변경시 UI 반응.
	OnStageIdChanged.Broadcast(CurrentStageId);
}

void ARogueGameState::OnRep_EndReason()
{
	//클라에서 값 변경시 UI 반응.
	OnEndReasonChanged.Broadcast(EndReason);
}

void ARogueGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARogueGameState, bRunActive);
	DOREPLIFETIME(ARogueGameState, CurrentStageId);
	DOREPLIFETIME(ARogueGameState, EndReason);
}

void ARogueGameState::NotifyRunActiveChanged_Server()
{
	//서버 로컬에서도 이벤트를 받고 싶을 때.(서버도 한 플레이어 처럼 UI가 있을 수 있음.)
	OnRunActiveChanged.Broadcast();
}

void ARogueGameState::NotifyStageIdChanged_Server()
{
	//서버도 로컬에서 이벤트를 받고 싶을 때.
	OnStageIdChanged.Broadcast(CurrentStageId);
}

void ARogueGameState::NotifyEndReasonChanged_Server()
{
	OnEndReasonChanged.Broadcast(EndReason);
}
