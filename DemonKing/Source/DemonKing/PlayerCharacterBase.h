#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacterBase.generated.h"

UENUM(BlueprintType)
enum class ESkillSlot : uint8
{
	Q UMETA(DisplayName = "Q"),
	E UMETA(DisplayName = "E"),
	Shift UMETA(DisplayName = "Shift")
};

UCLASS()
class DEMONKING_API APlayerCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacterBase();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	void InputSkillQ();
	void InputSkillE();
	void InputSkillShift();

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

	// BP에서 애니메이션 노티파이 시점에 호출하기 좋게 public으로 둠
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void PerformSkillQHitCheck();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Cooldown")
	float SkillQCooldown = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Cooldown")
	float SkillECooldown = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Cooldown")
	float SkillShiftCooldown = 8.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Cost")
	float CurrentMana = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Cost")
	float SkillQManaCost = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Cost")
	float SkillEManaCost = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Cost")
	float SkillShiftManaCost = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	bool bIsCastingSkill = false;

	// Q 공격 판정 세팅
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Q")
	float SkillQRange = 180.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Q")
	float SkillQRadius = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Q")
	float SkillQDamage = 25.0f;

private:
	float LastSkillQTime = -1000.0f;
	float LastSkillETime = -1000.0f;
	float LastSkillShiftTime = -1000.0f;
public:
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void SetCastingSkill(bool bNewCasting);
};