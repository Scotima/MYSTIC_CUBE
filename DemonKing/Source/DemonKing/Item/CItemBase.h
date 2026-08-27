// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CItemBase.generated.h"

UENUM(BlueprintType)
enum class EItemOptionType : uint8
{
	AttackPower UMETA(DisplayName = "Attack Power"),
	MaxHp UMETA(DisplayName = "Max HP"),
	Armor UMETA(DisplayName = "Armor"),
	AttackSpeed UMETA(DisplayName = "Attack Speed"),
	CritChance UMETA(DisplayName = "Crit Chance"),
	CritDamage UMETA(DisplayName = "Crit Damage"),
	BasicAttackDamage UMETA(DisplayName = "Basic Attack Damage"),
	SkillDamage UMETA(DisplayName = "Skill Damage"),
	SkillHaste UMETA(DisplayName = "Skill Haste"),
	DamageAmp UMETA(DisplayName = "Damage Amp")
};

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	Common UMETA(DisplayName = "Common"),
	Rare UMETA(DisplayName = "Rare"),
	Epic UMETA(DisplayName = "Epic")
};

USTRUCT(BlueprintType)
struct FItemOptionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EItemOptionType OptionType = EItemOptionType::AttackPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EItemRarity Rarity = EItemRarity::Common;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float Value = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 Weight = 100;
};

UCLASS()
class DEMONKING_API ACItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACItemBase();


	UFUNCTION(BlueprintCallable, Category = "Item")
	void StartItemSelection(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category = "Item")
	void SelectItemOption(int32 OptionIndex);

	UFUNCTION(BlueprintCallable, Category = "Item")
	void ApplyItemOption(AActor* TargetActor, const FItemOptionData& Option);

	UFUNCTION(BlueprintImplementableEvent, Category = "Item")
	void BP_ShowItemOptions(const TArray<FItemOptionData>& Options);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Category = "Item")
	TArray<FItemOptionData> CurrentOptions;

	UPROPERTY()
	TObjectPtr<AActor> CachedPlayer;

	TArray<FItemOptionData> GetDefaultItemPool() const;
	TArray<FItemOptionData> RollItemOptions(int32 OptionCount) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Debug")
	bool bDebugStartSelectionOnBeginPlay = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Debug")
	bool bDebugAutoSelectFirstOption = false;

	void DebugTryStartItemSelection();

	FTimerHandle DebugStartSelectionTimerHandle;

	int32 DebugStartSelectionAttemptCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Debug")
	int32 DebugStartSelectionMaxAttempts = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Debug")
	float DebugStartSelectionRetryInterval = 0.2f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	FItemOptionData MakeItemOption(
		FName ItemId,
		const FString& DisplayName,
		const FString& Description,
		EItemOptionType OptionType,
		EItemRarity Rarity,
		float Value,
		int32 Weight
	) const;
};
