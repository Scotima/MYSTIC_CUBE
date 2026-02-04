#include "DemonKing/GameFlow/RogueGameState.h"
#include "Net/UnrealNetwork.h"  
ARogueGameState::ARogueGameState()
{
	bReplicates = true;
}

void ARogueGameState::SetRunActive(bool bInRunActive)
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

void ARogueGameState::SetStageMapIndex(int32 NewIndex)
{
	if (!HasAuthority())
	{
		return;
	}

	const int32 Clamped = FMath::Max(0, NewIndex);

	if (StageMapIndex == Clamped)
	{
		return; // 중복 방지.
	}

	StageMapIndex = Clamped;
	NotifyStageMapIndexChanged_Server();
}

void ARogueGameState::SetRunSeed(int32 NewSeed)
{
	if (!HasAuthority())
	{
		return;
	}

	if (RunSeed == NewSeed)
	{
		return; // 중복 방지.
	}

	RunSeed = NewSeed;

	NotifyRunSeedChanged_Server();
}

void ARogueGameState::SetStageSeed(int32 NewSeed)
{
	if (!HasAuthority())
	{
		return;
	}

	if (StageSeed == NewSeed)
	{
		return; // 중복방지
	}

	StageSeed = NewSeed;
	NotifyStageSeedChanged_Server();
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

	StageMapIndex = 0;

	NotifyRunActiveChanged_Server();
	NotifyStageIdChanged_Server();
	NotifyEndReasonChanged_Server();
	NotifyStageMapIndexChanged_Server();
	NotifyRunSeedChanged_Server();
	NotifyStageSeedChanged_Server();
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

void ARogueGameState::OnRep_StageMapIndex()
{
	OnStageMapIndexChanged.Broadcast(StageMapIndex);
}

void ARogueGameState::OnRep_RunSeed()
{
	OnRunSeedChanged.Broadcast(RunSeed);
}

void ARogueGameState::OnRep_StageSeed()
{
	OnStageSeedChanged.Broadcast(StageSeed);
}

void ARogueGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARogueGameState, bRunActive);
	DOREPLIFETIME(ARogueGameState, CurrentStageId);
	DOREPLIFETIME(ARogueGameState, EndReason);

	DOREPLIFETIME(ARogueGameState, StageMapIndex);
	DOREPLIFETIME(ARogueGameState, RunSeed);
	DOREPLIFETIME(ARogueGameState, StageSeed);
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

void ARogueGameState::NotifyStageMapIndexChanged_Server()
{
	OnStageMapIndexChanged.Broadcast(StageMapIndex);
}

void ARogueGameState::NotifyRunSeedChanged_Server()
{
	OnRunSeedChanged.Broadcast(RunSeed);
}

void ARogueGameState::NotifyStageSeedChanged_Server()
{
	OnStageSeedChanged.Broadcast(StageSeed);
}
