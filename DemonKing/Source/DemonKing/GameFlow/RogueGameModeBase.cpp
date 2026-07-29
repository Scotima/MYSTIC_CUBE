#include "DemonKing/GameFlow/RogueGameModeBase.h"
#include "Engine/World.h"
#include "DemonKing/GameFlow/RogueGameState.h"
#include "DemonKing/SaveGame/RogueSaveSubsystem.h"
#include "GameFramework/PlayerController.h"

ARogueGameModeBase::ARogueGameModeBase()
{

    bUseSeamlessTravel = true;


    GameStateClass = ARogueGameState::StaticClass();
    

    //스테이지 이름 미정일 시 기본 3단계로 고정.
    StageOrder = { FName("Stage_01"), FName("Stage_02"), FName("Stage_03") };

    // 실제 맵이 생기면 아래처럼 채우면 됨:
    // StageToMaps["stage1"].MapPaths = { "/Game/Maps/stage1-1", "/Game/Maps/stage1-2" ... }
    {
        FStageMapList List1;
        List1.MapPaths = { FName(*DefaultRunMapPath) };
        StageToMaps.Add(FName("Stage_01"), List1);

        FStageMapList List2;
        List2.MapPaths = { FName(*DefaultRunMapPath) };
        StageToMaps.Add(FName("Stage_02"), List2);

        FStageMapList List3;
        List3.MapPaths = { FName(*DefaultRunMapPath) };
        StageToMaps.Add(FName("Stage_03"),List3);
    }



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

    APlayerController* PC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;
    URogueSaveSubsystem* SaveSS = GetSaveSS();

    FName StageId = FName("Stage_01");
    int32 StageMapIndex = 0;
    int32 RunSeed = 0;// 안쓰는거
    int32 StageSeed = 0;//안쓰는거

    const bool bResumed = (SaveSS) ? SaveSS->LoadOrStartNewRun(PC, StageId, StageMapIndex, RunSeed, StageSeed) : false;

    GS->ResetRunState();
    StageStep_Runtime = 0;
    GS->SetStageId(StageId);
    GS->SetEndReason(ERogueRunEndReason::None);
    GS->SetRunActive(true);

    //GS->SetRunSeed(RunSeed);
    GS->SetStageMapIndex(StageMapIndex);
    //GS->SetStageSeed(StageSeed);



  
    UE_LOG(LogTemp, Warning, TEXT("StartRun: %s Stage=%s MapIndex=%d RunSeed=%d StageSeed=%d"),
        bResumed ? TEXT("RESUME") : TEXT("NEW"),
        *StageId.ToString(), StageMapIndex, RunSeed, StageSeed);

    ServerTravelToStage(StageId, StageMapIndex);

   
}

void ARogueGameModeBase::AdvanceStage() // MapIndex 종료되면 다음 스테이지로 넘어감. // 넘어 갈 때 호출. // 즉 한 스테이지 당 한번만 호출하면 됨.
{
    if (!EnsureServerAuth(TEXT("AdvaceStage")))
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
    //const FName Next = GetNextStageId(Current);
    int32 index = 0;
    index++;
    FName Next = StageOrder[index]; //StageOrder의 필요성? 추후 그냥 CSV로 해도 되려나..

    //마지막 스테이지 이후면 보스킬/성공 처리 (원하는 정책으로 변경 가능)
    if (Next.IsNone())
    {
        EndRun(ERogueRunEndReason::BossKilled);
        return;
    }
    GS->SetStageMapIndex(0);//어차피 다음 스테이지로 넘어가는거면 초기화 해도 될듯.
    GS->SetStageId(Next);

    //const int32 RunSeed = GS->GetRunSeed();
    //const int32 NextStageIndex = GetStageOrderIndexSafe(Next);

    //// 다음 스테이지 첫 맵도 시드 기반 선택.

    //const int32 PickedMapIndex = PickStageMapIndex_Seeded(Next, RunSeed, NextStageIndex, StageStep_Runtime, GS->GetStageMapIndex());
    //GS->SetStageMapIndex(PickedMapIndex);

    //const int32 StageSeed = MakeStageSeed_Seeded(RunSeed, NextStageIndex, StageStep_Runtime);
    //GS->SetStageSeed(StageSeed);

    //GS->SetStageMapIndex(GS->GetStageMapIndex());

    //다음 스테이지로 트래블(현재는 L_Run 고정)
    ServerTravelToStage(Next, GS->GetStageMapIndex());


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
// 같은 스테이지 내부 다음 맵으로 이동(포탈 UI에서 예를 누르면 호출)
void ARogueGameModeBase::AdvanceMapWithinStage() // 맵 포탈 기능 구현시 그쪽 클래스에서 호출.
{
    if (!EnsureServerAuth(TEXT("AdvanceMapWithinStage")))
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
        UE_LOG(LogTemp, Warning, TEXT("AdvanceMapWithinStage ignored: Run is not active"));
        return;
    }

    const FName CurrentStage = GS->GetStageId(); // 스테이지 Id를 가져옴

    //현재 스테이지의 맵 리스트 가져오기
    const FStageMapList* List = StageToMaps.Find(CurrentStage); //근데 맵 리스트에 대한 정보가 부족하지 않나

    int32 StageIndex = GS->GetStageMapIndex(); // 맵 리스트도 같이 정리.
    StageIndex++;

    if (!List || List->MapPaths.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("AdvanceMapWithinStage failed: No MapPaths for StageId=%s"), *CurrentStage.ToString());
        return;
    }

    //스테이지 내부 스텝을 올리고, 시드 기반으로 맵을 다시 선택 + 맵이 그대로여도 StageSeed는 매번 바뀌어서 콘텐츠가 달라짐 // 시드 자체가 필요한지 의문 그냥 단계
    //이어가는 방식 또한 괜찮아 보임.
   /* StageStep_Runtime++;

    const int32 RunSeed = GS->GetRunSeed();
    const int32 StageIndex = GetStageOrderIndexSafe(CurrentStage);

    const int32 CurrentMapIndex = GS->GetStageMapIndex();
    const int32 PickedMapIndex = PickStageMapIndex_Seeded(
        CurrentStage,
        RunSeed,
        StageIndex,
        StageStep_Runtime,
        CurrentMapIndex);

    GS->SetStageMapIndex(PickedMapIndex);*/

    //같은 맵이여도 콘텐츠 랜덤을 바꾸는 핵심
    //const int32 StageSeed = MakeStageSeed_Seeded(RunSeed, StageIndex, StageStep_Runtime);
    //GS->SetStageSeed(StageSeed);

    ServerTravelToStage(CurrentStage, GS->GetStageMapIndex());

}


void ARogueGameModeBase::StartPlay()
{
    Super::StartPlay();

    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("[GM StartPlay] World=null"));
        return;
    }

    // ✅ [추가] 월드/맵/넷모드/URL
    UE_LOG(LogTemp, Warning, TEXT("[GM StartPlay] Map=%s World=%s NetMode=%d URL=%s"),
        *World->GetMapName(),
        *World->GetName(),
        (int32)World->GetNetMode(),
        *World->URL.ToString());

    // ✅ [추가] GameMode 클래스 확인 (BP로 올라왔는지)
    UE_LOG(LogTemp, Warning, TEXT("[GM StartPlay] GameModeClass=%s"), *GetClass()->GetName());

    // ✅ [추가] SeamlessTravel 여부 (PC 유지되는지 원인 확인)
    UE_LOG(LogTemp, Warning, TEXT("[GM StartPlay] bUseSeamlessTravel=%s"),
        bUseSeamlessTravel ? TEXT("true") : TEXT("false"));

    // ✅ [추가] PC 상태 확인 (MenuPC가 유지되는지 + LocalPlayer 붙었는지)
    if (APlayerController* PC = World->GetFirstPlayerController())
    {
        const bool bHasPlayer = (PC->Player != nullptr); // UPlayer* (LocalPlayer면 여기 들어옴)
        UE_LOG(LogTemp, Warning, TEXT("[GM StartPlay] FirstPC=%s HasPlayer=%s IsLocal=%s"),
            *PC->GetClass()->GetName(),
            bHasPlayer ? TEXT("true") : TEXT("false"),
            PC->IsLocalController() ? TEXT("true") : TEXT("false"));

        // ✅ [추가] Pawn 존재 여부
        APawn* Pawn = PC->GetPawn();
        UE_LOG(LogTemp, Warning, TEXT("[GM StartPlay] PC Pawn=%s"),
            Pawn ? *Pawn->GetName() : TEXT("null"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[GM StartPlay] FirstPlayerController=null"));
    }

    // (원래 마스터 코드에 있던 GS 로그도 유지 가능)
    if (AGameStateBase* GS = GetRogueGS())
    {
        UE_LOG(LogTemp, Warning, TEXT("[GM StartPlay] GS Class=%s"), *GS->GetClass()->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[GM StartPlay] GS is null"));
    }
}

ARogueGameState* ARogueGameModeBase::GetRogueGS() const
{
    return GetGameState<ARogueGameState>();
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

void ARogueGameModeBase::ServerTravelToStage(const FName& StageId, int32 StageMapIndex)
{
    if (UWorld* World = GetWorld())
    {
        const FStageMapList* List = StageToMaps.Find(StageId); //TMap에서 정보를 하나씩 꺼내기 위한 코드.

        //매핑이 없거나 비었으면 DefaultRunMapPath로 fallback

        if (!List || List->MapPaths.IsEmpty())
        {
            UE_LOG(LogTemp, Warning, TEXT("No MapPaths for %s. Fallback to DefaultRunMapPath=%s"), *StageId.ToString(), *DefaultRunMapPath);

            World->ServerTravel(DefaultRunMapPath);
            return;
        }


        //인덱스 안전 처리.

        if (!List->MapPaths.IsValidIndex(StageMapIndex)) // 안전하게 처리하기 위함.
        {
            return;
        }

        const FString Target = List->MapPaths[StageMapIndex].ToString();

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

//시드 /[랜덤]헬퍼 구현

int32 ARogueGameModeBase::GetStageOrderIndexSafe(const FName& StageId) const
{
    const int32 Idx = StageOrder.IndexOfByKey(StageId);
    return (Idx == INDEX_NONE) ? 0 : Idx;
}

URogueSaveSubsystem* ARogueGameModeBase::GetSaveSS() const
{
    if (UGameInstance* GI = GetGameInstance())
    {
        return GI->GetSubsystem<URogueSaveSubsystem>();

    }

    return nullptr;
}
