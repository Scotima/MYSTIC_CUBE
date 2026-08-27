#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DemonKing/GameFlow/MyGameInstance.h"
#include "CCharacterStatComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )

class DEMONKING_API UCCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCCharacterStatComponent();
protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Stat")
	void InitByClass(EPlayerClassType ClassType);

	UFUNCTION(BlueprintCallable, Category = "Stat")
	void SetBaseStats(float NewMaxHp, float NewArmor, float NewAttackPower, float NewAttackSpeed);

	UFUNCTION(BlueprintCallable, Category = "Stat")
	void TakeDamage(float IncomingDamage);

	UFUNCTION(BlueprintCallable, Category = "Stat")
	void Heal(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Stat")
	void AddMaxHp(float Amount, bool bHealByAddedAmount = true);

	UFUNCTION(BlueprintPure, Category = "Stat")
	float GetMaxHp() const;

	UFUNCTION(BlueprintPure, Category = "Stat")
	float GetCurrentHp() const;

	void Die();

	UFUNCTION(BlueprintPure, Category = "Stat|Combat")
	float GetFinalAttackPower() const;
	FORCEINLINE float GetHP_Percent() { return CurrentHp >= 0 ? CurrentHp / MaxHp : 0; }

	//블루프린트에서도 할 수 있게..
	//공식은 같지만 마법사 체력이 낮고
	//전사니까 체력이 높고

	UFUNCTION(BlueprintPure, Category = "Stat|Combat")
	float GetFinalAttackSpeed() const;

	UFUNCTION(BlueprintPure, Category = "Stat|Combat")
	float GetAttackInterval() const;

	UFUNCTION(BlueprintPure, Category = "Stat|Combat")
	float GetFinalCooldown(float BaseCooldown) const;

	UFUNCTION(BlueprintPure, Category = "Stat|Combat")
	float CalculateSkillDamage(float SkillCoefficient) const;

	UFUNCTION(BlueprintPure, Category = "Stat|Combat")
	float CalculateHealAmount(float HealCoefficient) const;

	float CalculateBasicAttackDamage(bool& bOutCrit) const;

	UFUNCTION(BlueprintPure, Category = "Stat|Defense")
	float GetFinalArmor() const;

	UFUNCTION(BlueprintPure, Category = "Stat|Defense")
	float GetDamageReduction() const;

	UFUNCTION(BlueprintCallable, Category = "Stat|Modifier")
	void AddArmorBonusRate(float BonusRate);

	UFUNCTION(BlueprintCallable, Category = "Stat|Modifier")
	void RemoveArmorBonusRate(float BonusRate);

	UFUNCTION(BlueprintCallable, Category = "Stat|Modifier")
	void AddAttackSpeedIncrease(float IncreaseRate);

	UFUNCTION(BlueprintCallable, Category = "Stat|Modifier")
	void RemoveAttackSpeedIncrease(float IncreaseRate);

	UFUNCTION(BlueprintCallable, Category = "Stat|Modifier")
	void AddAttackPowerIncrease(float IncreaseRate);

	UFUNCTION(BlueprintCallable, Category = "Stat|Modifier")
	void RemoveAttackPowerIncrease(float IncreaseRate);

	UFUNCTION(BlueprintCallable, Category = "Stat|Modifier")
	void AddBasicAttackDamageIncrease(float IncreaseRate);

	UFUNCTION(BlueprintCallable, Category = "Stat|Modifier")
	void RemoveBasicAttackDamageIncrease(float IncreaseRate);

	UFUNCTION(BlueprintCallable, Category = "Stat|Modifier")
	void AddSkillDamageIncrease(float IncreaseRate);

	UFUNCTION(BlueprintCallable, Category = "Stat|Modifier")
	void RemoveSkillDamageIncrease(float IncreaseRate);

	UFUNCTION(BlueprintCallable, Category = "Stat|Modifier")
	void AddDamageAmp(float IncreaseRate);

	UFUNCTION(BlueprintCallable, Category = "Stat|Modifier")
	void RemoveDamageAmp(float IncreaseRate);

	UFUNCTION(BlueprintCallable, Category = "Stat|Modifier")
	void AddCritChance(float IncreaseRate);

	UFUNCTION(BlueprintCallable, Category = "Stat|Modifier")
	void RemoveCritChance(float IncreaseRate);

	UFUNCTION(BlueprintCallable, Category = "Stat|Modifier")
	void AddCritDamageIncrease(float IncreaseRate);

	UFUNCTION(BlueprintCallable, Category = "Stat|Modifier")
	void RemoveCritDamageIncrease(float IncreaseRate);

	UFUNCTION(BlueprintCallable, Category = "Stat|Modifier")
	void AddSkillHaste(float HasteAmount);

	UFUNCTION(BlueprintCallable, Category = "Stat|Modifier")
	void RemoveSkillHaste(float HasteAmount);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat|Base")
	EPlayerClassType DefaultClassType = EPlayerClassType::Warrior;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat|Base")
	float MaxHp = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat|Base")
	float CurrentHp = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat|Base")
	float BaseArmor = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat|Base")
	float BaseAttackPower = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat|Base")
	float BaseAttackSpeed = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat|Modifier")
	float AttackPowerIncreaseSum = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat|Modifier")
	float BasicAttackDamageIncreaseSum = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat|Modifier")
	float SkillDamageIncreaseSum = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat|Modifier")
	float DamageAmpSum = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat|Modifier", meta=(ClampMin="0.0", ClampMax="1.0"))
	float CritChance = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat|Modifier")
	float CritDamageIncreaseSum = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat|Modifier")
	float AttackSpeedIncreaseSum = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat|Modifier", meta=(ClampMin="0.0", ClampMax="300.0"))
	float SkillHaste = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat|Modifier")
	float ArmorBonusRateSum = 0.0f;

private:
	bool isDead = false;
	bool bCanBeDamaged = true;

	static constexpr float ArmorK = 200.0f;
	static constexpr float MaxDamageReduction = 0.8f;
	static constexpr float MaxAttackSpeed = 5.0f;
	static constexpr float AttackSpeedOverflowEfficiency = 0.35f;
	static constexpr float MaxSkillHaste = 300.0f;
};
