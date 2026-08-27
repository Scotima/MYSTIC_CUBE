#pragma once

#include "CoreMinimal.h"
#include "CCharacter/RogueCharacterBase.h"
#include "PlayerCharacterBase.generated.h"

UENUM(BlueprintType)
enum class ESkillSlot : uint8
{
	Q UMETA(DisplayName = "Q"),
	E UMETA(DisplayName = "E"),
	Shift UMETA(DisplayName = "Shift")
};

UCLASS()
class DEMONKING_API APlayerCharacterBase : public ARogueCharacterBase
{
	GENERATED_BODY()

public:
	APlayerCharacterBase();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void InputSkillQ() override;
	virtual void InputSkillE() override;
	virtual void InputSkillShift() override;
	virtual void InputSkillLeftMouse() override;
	virtual void InputSkillLeftMouseReleased() override;

	bool CanUseSkill(ESkillSlot SkillSlot) const;
	void CommitSkill(ESkillSlot SkillSlot);
	float GetCooldownRemaining(ESkillSlot SkillSlot) const;

public:
	// BP에서 구현할 이벤트
	UFUNCTION(BlueprintImplementableEvent, Category = "Skill")
	void BP_UseSkillQ();

	UFUNCTION(BlueprintImplementableEvent, Category = "Skill")
	void BP_UseSkillE();

	UFUNCTION(BlueprintImplementableEvent, Category = "Skill")
	void BP_UseSkillShift();

	UFUNCTION(BlueprintImplementableEvent, Category = "Skill")
	void BP_UseLeftMouseSkill();

	UFUNCTION(BlueprintImplementableEvent, Category = "Skill")
	void BP_LeftMousePressed();

	UFUNCTION(BlueprintImplementableEvent, Category = "Skill")
	void BP_LeftMouseReleased();

	// BP에서 애니메이션 노티파이 시점에 호출하기 좋게 public으로 둠
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void PerformSkillQHitCheck();
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void PerformSkillEHitCheck();

	UFUNCTION(BlueprintCallable, Category = "Skill")
	void ActivateShieldSkill();

	UFUNCTION(BlueprintCallable, Category = "Skill")
	void EndShieldSkill();

	UFUNCTION(BlueprintCallable, Category = "Skill|Targeting")
	bool CacheMouseSkillTarget(ESkillSlot SkillSlot, float MaxCastRange);

	UFUNCTION(BlueprintPure, Category = "Skill|Targeting")
	FVector GetCachedSkillTargetLocation(ESkillSlot SkillSlot) const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Cooldown")
	float SkillQCooldown = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Cooldown")
	float SkillECooldown = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Cooldown")
	float SkillShiftCooldown = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Cost")
	float CurrentMana = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Cost")
	float SkillQManaCost = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Cost")
	float SkillEManaCost = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Cost")
	float SkillShiftManaCost = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	bool bIsCastingSkill = false;

	// Q 공격 판정 세팅
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Q")
	float SkillQRange = 180.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Q")
	float SkillQRadius = 80.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Q")
	float SkillQDamage = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|E")
	float SkillERange = 120.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|E")
	float SkillERadius = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|E")
	float SkillEDamage = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Shift")
	bool bShieldActive = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Shift")
	float MaxShieldAmount = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Shift")
	float CurrentShieldAmount = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Shift")
	float ShieldDuration = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Q")
	bool bSkillQUsesMouseTarget = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Q")
	float SkillQCastRange = 800.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|E")
	bool bSkillEUsesMouseTarget = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|E")
	float SkillECastRange = 800.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Shift")
	bool bSkillShiftUsesMouseTarget = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Shift")
	float SkillShiftCastRange = 800.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Skill|Targeting")
	FVector CachedSkillQTargetLocation = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Category = "Skill|Targeting")
	FVector CachedSkillETargetLocation = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Category = "Skill|Targeting")
	FVector CachedSkillShiftTargetLocation = FVector::ZeroVector;

	FTimerHandle ShieldTimerHandle;

private:
	float LastSkillQTime = -1000.0f;
	float LastSkillETime = -1000.0f;
	float LastSkillShiftTime = -1000.0f;
public:
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void SetCastingSkill(bool bNewCasting);
};

