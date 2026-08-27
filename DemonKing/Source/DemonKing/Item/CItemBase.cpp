// Fill out your copyright notice in the Description page of Project Settings.
#include "DemonKing/Item/CItemBase.h"
#include "DemonKing/ActorComponent/PlayerComponent/CCharacterStatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"

// Sets default values
ACItemBase::ACItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACItemBase::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("[CItemBase] BeginPlay: %s / DebugStart=%s / AutoSelect=%s"),
		*GetName(),
		bDebugStartSelectionOnBeginPlay ? TEXT("true") : TEXT("false"),
		bDebugAutoSelectFirstOption ? TEXT("true") : TEXT("false")
	);

	if (bDebugStartSelectionOnBeginPlay)
	{
		DebugStartSelectionAttemptCount = 0;

		GetWorldTimerManager().SetTimer(
			DebugStartSelectionTimerHandle,
			this,
			&ACItemBase::DebugTryStartItemSelection,
			DebugStartSelectionRetryInterval,
			true
		);
	}
}

// Called every frame
void ACItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACItemBase::StartItemSelection(AActor* TargetActor)
{
	UE_LOG(LogTemp, Warning, TEXT("[CItemBase] StartItemSelection Called"));

	if (!TargetActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("[CItemBase] StartItemSelection Failed: TargetActor is null"));
		return;
	}

	CachedPlayer = TargetActor;
	CurrentOptions = RollItemOptions(3);

	for (int32 Index = 0; Index < CurrentOptions.Num(); ++Index)
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("Item Option %d: %s / %s"),
			Index,
			*CurrentOptions[Index].DisplayName.ToString(),
			*CurrentOptions[Index].Description.ToString()
		);
	}

	BP_ShowItemOptions(CurrentOptions);

	if (bDebugAutoSelectFirstOption)
	{
		SelectItemOption(0);
	}
}

void ACItemBase::SelectItemOption(int32 OptionIndex)
{
	if (!CachedPlayer || !CurrentOptions.IsValidIndex(OptionIndex))
	{
		return;
	}

	ApplyItemOption(CachedPlayer, CurrentOptions[OptionIndex]);
	Destroy();
}

void ACItemBase::ApplyItemOption(AActor* TargetActor, const FItemOptionData& Option)
{
	if (!TargetActor)
	{
		return;
	}

	UCCharacterStatComponent* Stat = TargetActor->FindComponentByClass<UCCharacterStatComponent>();
	if (!Stat)
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("[CItemBase] ApplyItemOption Failed: StatComponent not found on %s"),
			*TargetActor->GetName()
		);
		return;
	}

	


	switch (Option.OptionType)
	{
	case EItemOptionType::AttackPower:
		Stat->AddAttackPowerIncrease(Option.Value);
		break;
	case EItemOptionType::MaxHp:
		Stat->AddMaxHp(Option.Value, true);
		break;
	case EItemOptionType::Armor:
		Stat->AddArmorBonusRate(Option.Value);
		break;
	case EItemOptionType::AttackSpeed:
		Stat->AddAttackSpeedIncrease(Option.Value);
		break;
	case EItemOptionType::CritChance:
		Stat->AddCritChance(Option.Value);
		break;
	case EItemOptionType::CritDamage:
		Stat->AddCritDamageIncrease(Option.Value);
		break;
	case EItemOptionType::BasicAttackDamage:
		Stat->AddBasicAttackDamageIncrease(Option.Value);
		break;
	case EItemOptionType::SkillDamage:
		Stat->AddSkillDamageIncrease(Option.Value);
		break;
	case EItemOptionType::SkillHaste:
		Stat->AddSkillHaste(Option.Value);
		break;
	case EItemOptionType::DamageAmp:
		Stat->AddDamageAmp(Option.Value);
		break;
	default:
		break;
	}

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("Applied Item Option: %s / Value: %.2f"),
		*Option.DisplayName.ToString(),
		Option.Value
	);


}

FItemOptionData ACItemBase::MakeItemOption(
	FName ItemId,
	const FString& DisplayName,
	const FString& Description,
	EItemOptionType OptionType,
	EItemRarity Rarity,
	float Value,
	int32 Weight
) const
{
	FItemOptionData Option;
	Option.ItemId = ItemId;
	Option.DisplayName = FText::FromString(DisplayName);
	Option.Description = FText::FromString(Description);
	Option.OptionType = OptionType;
	Option.Rarity = Rarity;
	Option.Value = Value;
	Option.Weight = Weight;

	return Option;
}

TArray<FItemOptionData> ACItemBase::GetDefaultItemPool() const
{
	TArray<FItemOptionData> Pool;

	Pool.Add(MakeItemOption(
		TEXT("AttackPower_01"),
		TEXT("공격력 증가"),
		TEXT("공격력이 10% 증가합니다."),
		EItemOptionType::AttackPower,
		EItemRarity::Common,
		0.10f,
		100
	));

	Pool.Add(MakeItemOption(
		TEXT("MaxHp_01"),
		TEXT("최대 체력 증가"),
		TEXT("최대 체력이 50 증가합니다."),
		EItemOptionType::MaxHp,
		EItemRarity::Common,
		50.0f,
		100
	));

	Pool.Add(MakeItemOption(
		TEXT("Armor_01"),
		TEXT("방어력 증가"),
		TEXT("방어력이 15% 증가합니다."),
		EItemOptionType::Armor,
		EItemRarity::Common,
		0.15f,
		100
	));

	Pool.Add(MakeItemOption(
		TEXT("AttackSpeed_01"),
		TEXT("공격 속도 증가"),
		TEXT("공격 속도가 8% 증가합니다."),
		EItemOptionType::AttackSpeed,
		EItemRarity::Common,
		0.08f,
		100
	));

	Pool.Add(MakeItemOption(
		TEXT("CritChance_01"),
		TEXT("치명타 확률 증가"),
		TEXT("치명타 확률이 5% 증가합니다."),
		EItemOptionType::CritChance,
		EItemRarity::Rare,
		0.05f,
		60
	));

	Pool.Add(MakeItemOption(
		TEXT("SkillDamage_01"),
		TEXT("스킬 피해 증가"),
		TEXT("스킬 피해량이 12% 증가합니다."),
		EItemOptionType::SkillDamage,
		EItemRarity::Rare,
		0.12f,
		60
	));

	return Pool;
}

TArray<FItemOptionData> ACItemBase::RollItemOptions(int32 OptionCount) const
{
	TArray<FItemOptionData> Pool = GetDefaultItemPool();
	TArray<FItemOptionData> Result;

	const int32 TargetCount = FMath::Min(OptionCount, Pool.Num());

	while (Result.Num() < TargetCount && Pool.Num() > 0)
	{
		int32 TotalWeight = 0;

		for (const FItemOptionData& Option : Pool)
		{
			TotalWeight += FMath::Max(0, Option.Weight);
		}

		if (TotalWeight <= 0)
		{
			const int32 RandomIndex = FMath::RandRange(0, Pool.Num() - 1);
			Result.Add(Pool[RandomIndex]);
			Pool.RemoveAt(RandomIndex);
			continue;
		}

		int32 Roll = FMath::RandRange(1, TotalWeight);
		int32 SelectedIndex = 0;

		for (int32 Index = 0; Index < Pool.Num(); ++Index)
		{
			Roll -= FMath::Max(0, Pool[Index].Weight);

			if (Roll <= 0)
			{
				SelectedIndex = Index;
				break;
			}
		}

		Result.Add(Pool[SelectedIndex]);
		Pool.RemoveAt(SelectedIndex);
	}

	return Result;
}


void ACItemBase::DebugTryStartItemSelection()
{
	DebugStartSelectionAttemptCount++;

	AActor* TargetActor = nullptr;

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		APawn* Pawn = PC->GetPawn();

		UE_LOG(
			LogTemp,
			Warning,
			TEXT("[CItemBase] Debug PC=%s / Pawn=%s"),
			*PC->GetName(),
			Pawn ? *Pawn->GetName() : TEXT("None")
		);

		if (Pawn && Pawn->FindComponentByClass<UCCharacterStatComponent>())
		{
			TargetActor = Pawn;
		}
	}

	if (!TargetActor)
	{
		if (UWorld* World = GetWorld())
		{
			for (TActorIterator<AActor> It(World); It; ++It)
			{
				AActor* Actor = *It;

				if (Actor && Actor != this && Actor->FindComponentByClass<UCCharacterStatComponent>())
				{
					TargetActor = Actor;
					break;
				}
			}
		}
	}

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("[CItemBase] DebugTryStartItemSelection Attempt=%d / Target=%s"),
		DebugStartSelectionAttemptCount,
		TargetActor ? *TargetActor->GetName() : TEXT("None")
	);

	if (!TargetActor)
	{
		if (DebugStartSelectionAttemptCount >= DebugStartSelectionMaxAttempts)
		{
			GetWorldTimerManager().ClearTimer(DebugStartSelectionTimerHandle);
			UE_LOG(LogTemp, Warning, TEXT("[CItemBase] Debug selection failed: target actor not found"));
		}

		return;
	}

	GetWorldTimerManager().ClearTimer(DebugStartSelectionTimerHandle);
	StartItemSelection(TargetActor);
}
