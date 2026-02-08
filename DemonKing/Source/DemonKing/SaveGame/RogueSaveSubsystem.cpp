#include "DemonKing/SaveGame/RogueSaveSubsystem.h"
#include "DemonKing/SaveGame/RogueSaveGame.h"

#include "DemonKing/GameFlow/RogueGameState.h"
#include "Kismet/GameplayStatics.h"
#include"GameFramework/PlayerController.h"
#include "GameFramework/Character.h"

#include "Engine/World.h"
#include "TimerManager.h"

bool URogueSaveSubsystem::HasRunSave() const
{
	return UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex);
}

URogueSaveGame* URogueSaveSubsystem::LoadRunSave()
{
	if (!HasRunSave())
	{
		return nullptr; //세이브 저장 파일이 없음.
	}

	USaveGame* Loaded = UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex);
	URogueSaveGame* SaveObj = Cast<URogueSaveGame>(Loaded);

	CachedSave = SaveObj;
	return SaveObj;

}

bool URogueSaveSubsystem::WriteRunSave(URogueSaveGame* SaveObj)
{
	if (!SaveObj)
	{
		return false;
	}

	CachedSave = SaveObj;
	return UGameplayStatics::SaveGameToSlot(SaveObj, SlotName, UserIndex);
}

void URogueSaveSubsystem::DeleteRunSave()
{
	CachedSave = nullptr;
	if (HasRunSave())
	{
		UGameplayStatics::DeleteGameInSlot(SlotName, UserIndex);
	}
}

void URogueSaveSubsystem::SaveOnQuit(APlayerController* PC)
{
	URogueSaveGame* SaveObj = CapureSnapshot(PC);
	if (!SaveObj)
	{
		return;
	}

	const bool bOk = WriteRunSave(SaveObj);
	UE_LOG(LogTemp, Warning, TEXT("SaveOnQuit: %s"), bOk ? TEXT("OK") : TEXT("Failed"));
}

bool URogueSaveSubsystem::LoadOrStartNewRun(APlayerController* PC, FName& OutStageId, int32& OutStageMapIndex, int32& OutRunSeed, int32& OutStageSeed)
{
	URogueSaveGame* Loaded = LoadRunSave();

	//저장 버전이 있다면 그거 로드하면 됨

	if (Loaded && Loaded->SaveVersion == ExpectedSaveVersion)
	{
		OutStageId = Loaded->StageId;
		OutStageMapIndex = Loaded->StageMapIndex;
		OutRunSeed = Loaded->RunSeed;
		OutStageSeed = Loaded->StageSeed;

		ApplySnapshot(PC, Loaded);
		return true;
	}

	//만약 저장해놓은것이 없다면

	if (Loaded && Loaded->SaveVersion != ExpectedSaveVersion)
	{
		UE_LOG(LogTemp, Warning, TEXT("SaveVersion mismatch -> deleting old save"));
		DeleteRunSave();
	}

	URogueSaveGame* NewSave = Cast<URogueSaveGame>(UGameplayStatics::CreateSaveGameObject(URogueSaveGame::StaticClass()));
	if (!NewSave)
	{
		OutStageId = FName("Stage01");
		OutStageMapIndex = 0;
		OutRunSeed = 0;
		OutStageSeed = 0;

		return false;
	}

	NewSave->SaveVersion = ExpectedSaveVersion;
	NewSave->StageId = FName("Stage_01");
	NewSave->StageMapIndex = 0;
	NewSave->RunSeed = FMath::Rand();
	NewSave->StageSeed = FMath::Rand();

	WriteRunSave(NewSave);

	OutStageId = NewSave->StageId;
	OutStageMapIndex = NewSave->StageMapIndex;
	OutRunSeed = NewSave->RunSeed;
	OutStageSeed = NewSave->StageSeed;

	return false;
}

URogueSaveGame* URogueSaveSubsystem::CapureSnapshot(APlayerController* PC) const
{
	UWorld* World = GetWorld();

	ARogueGameState* GS = World->GetGameState<ARogueGameState>();

	if (!GS)
	{
		return nullptr;
	}

	URogueSaveGame* SaveObj = Cast<URogueSaveGame>(UGameplayStatics::CreateSaveGameObject(URogueSaveGame::StaticClass()));

	if (!SaveObj)
	{
		return nullptr;
	}

	SaveObj->SaveVersion = ExpectedSaveVersion;
	SaveObj->StageMapIndex = GS->GetStageMapIndex();
	SaveObj->RunSeed = GS->GetRunSeed();
	SaveObj->StageSeed = GS->GetStageSeed();

	if (PC)
	{
		if (ACharacter* Char = Cast<ACharacter>(PC->GetPawn()))
		{
			SaveObj->bHasSavedTransform = true;
			SaveObj->PlayerTransform = Char->GetActorTransform();
		}
	}

	//골드같은 나머지 것들은 추가하면 세이브에 추가하기.

	return SaveObj;
}

void URogueSaveSubsystem::ApplySnapshot(APlayerController* PC, URogueSaveGame* SaveObj)
{
	if (!PC || !SaveObj)
	{
		return;
	}

	auto ApplyTransformIfReady = [PC, SaveObj]()
		{
			if (!SaveObj->bHasSavedTransform)
			{
				return;
			}

			if (ACharacter* Char = Cast<ACharacter>(PC->GetPawn()))
			{
				Char->SetActorTransform(SaveObj->PlayerTransform);
			}
		};

	ApplyTransformIfReady();

	if (!PC->GetPawn() && GetWorld())
	{
		FTimerHandle TmpHandle;
		GetWorld()->GetTimerManager().SetTimer(TmpHandle,
			FTimerDelegate::CreateLambda(ApplyTransformIfReady), 0.2f, false);
	}

	//아이템등 추가 컴포넌트 생기면 여기서 코드 작성하기.

}
