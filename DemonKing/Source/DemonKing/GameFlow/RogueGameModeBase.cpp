#include "DemonKing/GameFlow/RogueGameModeBase.h"
#include "Engine/World.h"

ARogueGameModeBase::ARogueGameModeBase()
{
    //스테이지 이름 미정일 시 기본 3단계로 고정.
    StageOrder = { FName("Stage_01"), FName("Stage_02"), FName("Stage_03") };

    //지금은 맵이 L_Run 하나 뿐 전부 L_Run으로 매핑(나중에 스테이지별 맵 생기면 여기만 수정.)
    StageToMapPath.Add(FName("Stage_01"), DefaultRunMapPath);
    StageToMapPath.Add(FName("Stage_02"), DefaultRunMapPath);
    StageToMapPath.Add(FName("Stage_03"), DefaultRunMapPath);

}

void ARogueGameModeBase::StartRun()
{
    if (!EnsureServerAuth(TEXT("StartRun")))
    {
        return;
    }

    ARogueGameState* GS = GetRogueGS();

    if (!GS)
    {
        return;
    }
    //이미 런 진행 중이면 무시.
    if (GS->GetRunActive())
    {
        UE_LOG(LogTemp, Warning, TEXT("StartRun ignored: Run already active."));
        return;
    }
    // 런 상태 리셋-> 1스테이지 세팅 -> 런 시작.
    GS->ResetRunState();
    const FName FirstStage = (StageOrder.Num() > 0) ? StageOrder[0] : FName("Stage_01");

    GS->SetStageId(FirstStage);
    GS->SetEndReason(ERogueRunEndReason::None);
    GS->SetRunActive(true); // 이게 맞나?

    // 첫 스테이지로 트래블.
    ServerTravelToStage(FirstStage);
}

void ARogueGameModeBase::AdvanceStage()
{
    if (!EnsureServerAuth(TEXT("AdvaneStage")))
    {
        return;
    }

    ARogueGameState* GS = GetRogueGS();

    if (!GS)
    {
        return;
    }

    if (!GS->GetRunActive())
    {
        UE_LOG(LogTemp, Warning, TEXT("AdvanceStage ignored: Run is not active"));
        return;
    }

    const FName Current = GS->GetStageId();
    const FName Next = GetNextStageId(Current);

    //마지막 스테이지 이후면 보스킬/성공 처리 (원하는 정책으로 변경 가능)
    if (Next.IsNone())
    {
        EndRun(ERogueRunEndReason::BossKilled);
        return;
    }

    GS->SetStageId(Next);

    //다음 스테이지로 트래블(현재는 L_Run 고정)
    ServerTravelToStage(Next);


}

void ARogueGameModeBase::EndRun(ERogueRunEndReason Reason)
{
    if (!EnsureServerAuth(TEXT("EndRun")))
    {
        return;
    }

    ARogueGameState* GS = GetRogueGS();

    if (!GS) return;

    // 종료 상태 기록
    GS->SetRunActive(false);
    GS->SetEndReason(Reason);

    // 일단은 로비로 복귀(원하면 여기서 세이브)
    if (UWorld* World = GetWorld())
    {
        World->ServerTravel(LobbyMapPath);
    }
}

ARogueGameState* ARogueGameModeBase::GetRogueGS() const
{
    return GetGameState<ARogueGameState>();;
}

FName ARogueGameModeBase::GetNextStageId(const FName& CurrentStageId) const
{
    const int32 Index = StageOrder.IndexOfByKey(CurrentStageId);
    if (Index == INDEX_NONE)
    {
        return (StageOrder.Num() > 0) ? StageOrder[0] : NAME_None;
    }

    const int32 NextIndex = Index + 1;

    return (StageOrder.IsValidIndex(NextIndex)) ? StageOrder[NextIndex] : NAME_None;
}

void ARogueGameModeBase::ServerTravelToStage(const FName& StageId)
{
    if (UWorld* World = GetWorld())
    {
        //매핑 없으면 DefaultRunMapPath로 간다.
        const FString* Found = StageToMapPath.Find(StageId);
        const FString& Target = Found ? *Found : DefaultRunMapPath;

        World->ServerTravel(Target);
    }
}

bool ARogueGameModeBase::EnsureServerAuth(const TCHAR* FuncName) const
{
    if (!HasAuthority())
    {
        UE_LOG(LogTemp, Warning, TEXT("%s called on non-authority."), FuncName);
        return false;
    }
    return true;
}
