#include "DemonKing/ActorComponent/PlayerComponent/CCharacterStatComponent.h"

UCCharacterStatComponent::UCCharacterStatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	if (MaxHp <= 0.0f)
	{
		InitByClass(DefaultClassType);
	}
	else if (CurrentHp <= 0.0f)
	{
		CurrentHp = MaxHp;
	}
}

void UCCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCCharacterStatComponent::InitByClass(EPlayerClassType ClassType)
{
	switch (ClassType)
	{
	case EPlayerClassType::Warrior:
		SetBaseStats(450.0f, 40.0f, 35.0f, 0.9f);
		break;
	case EPlayerClassType::Mage:
		SetBaseStats(300.0f, 10.0f, 48.0f, 0.0f);
		break;
	case EPlayerClassType::Archer:
		SetBaseStats(330.0f, 15.0f, 32.0f, 1.25f);
		break;
	default:
		break;
	}
}

void UCCharacterStatComponent::SetBaseStats(float NewMaxHp, float NewArmor, float NewAttackPower, float NewAttackSpeed)
{
	MaxHp = FMath::Max(0.0f, NewMaxHp);
	CurrentHp = MaxHp;
	BaseArmor = FMath::Max(0.0f, NewArmor);
	BaseAttackPower = FMath::Max(0.0f, NewAttackPower);
	BaseAttackSpeed = FMath::Max(0.0f, NewAttackSpeed);
	isDead = false;
	bCanBeDamaged = true;
}

float UCCharacterStatComponent::GetFinalAttackPower() const
{
	const float RawAttackSpeed = BaseAttackSpeed * (1.0f + AttackSpeedIncreaseSum);
	const float OverflowAttackSpeed = FMath::Max(0.0f, RawAttackSpeed - MaxAttackSpeed);
	const float OverflowAttackPowerIncrease = (OverflowAttackSpeed / MaxAttackSpeed) * AttackSpeedOverflowEfficiency;

	return BaseAttackPower * FMath::Max(0.0f, 1.0f + AttackPowerIncreaseSum + OverflowAttackPowerIncrease);
}

float UCCharacterStatComponent::GetFinalAttackSpeed() const
{
	if (BaseAttackSpeed <= 0.0f)
	{
		return 0.0f;
	}

	const float RawAttackSpeed = BaseAttackSpeed * (1.0f + AttackSpeedIncreaseSum);
	return FMath::Clamp(RawAttackSpeed, 0.0f, MaxAttackSpeed);
}

float UCCharacterStatComponent::GetAttackInterval() const
{
	const float FinalAttackSpeed = GetFinalAttackSpeed();
	return FinalAttackSpeed > 0.0f ? 1.0f / FinalAttackSpeed : 0.0f;
}

float UCCharacterStatComponent::GetFinalCooldown(float BaseCooldown) const
{
	const float ClampedSkillHaste = FMath::Clamp(SkillHaste, 0.0f, MaxSkillHaste);
	return FMath::Max(0.0f, BaseCooldown) / (1.0f + ClampedSkillHaste / 100.0f);
}

float UCCharacterStatComponent::CalculateBasicAttackDamage(bool& bOutCrit) const
{
	const float ClampedCritChance = FMath::Clamp(CritChance, 0.0f, 1.0f);
	const float CritDamageMultiplier = FMath::Max(1.0f, 2.0f + CritDamageIncreaseSum);

	bOutCrit = FMath::FRand() < ClampedCritChance;

	const float CritMultiplier = bOutCrit ? CritDamageMultiplier : 1.0f;

	return GetFinalAttackPower()
		* FMath::Max(0.0f, 1.0f + BasicAttackDamageIncreaseSum)
		* FMath::Max(0.0f, 1.0f + DamageAmpSum)
		* CritMultiplier;
}

float UCCharacterStatComponent::CalculateSkillDamage(float SkillCoefficient) const
{
	return GetFinalAttackPower()
		* FMath::Max(0.0f, SkillCoefficient)
		* FMath::Max(0.0f, 1.0f + SkillDamageIncreaseSum)
		* FMath::Max(0.0f, 1.0f + DamageAmpSum);
}

float UCCharacterStatComponent::CalculateHealAmount(float HealCoefficient) const
{
	return GetFinalAttackPower() * FMath::Max(0.0f, HealCoefficient);
}

float UCCharacterStatComponent::GetFinalArmor() const
{
	return BaseArmor * FMath::Max(0.0f, 1.0f + ArmorBonusRateSum);
}

float UCCharacterStatComponent::GetDamageReduction() const
{
	const float FinalArmor = GetFinalArmor();
	const float DamageReduction = FinalArmor / (FinalArmor + ArmorK);
	return FMath::Clamp(DamageReduction, 0.0f, MaxDamageReduction);
}

void UCCharacterStatComponent::TakeDamage(float IncomingDamage)
{
	if (isDead || !bCanBeDamaged || CurrentHp <= 0.0f)
	{
		return;
	}

	const float FinalDamage = FMath::Max(0.0f, IncomingDamage) * (1.0f - GetDamageReduction());
	CurrentHp = FMath::Clamp(CurrentHp - FinalDamage, 0.0f, MaxHp);

	if (CurrentHp <= 0.0f)
	{
		Die();
	}
}

void UCCharacterStatComponent::Heal(float Amount)
{
	if (isDead || Amount <= 0.0f)
	{
		return;
	}

	CurrentHp = FMath::Clamp(CurrentHp + Amount, 0.0f, MaxHp);
}

void UCCharacterStatComponent::AddMaxHp(float Amount, bool bHealByAddedAmount)
{
	if (Amount <= 0.0f)
	{
		return;
	}

	MaxHp = FMath::Max(0.0f, MaxHp + Amount);

	if (bHealByAddedAmount)
	{
		CurrentHp = FMath::Clamp(CurrentHp + Amount, 0.0f, MaxHp);
	}
	else
	{
		CurrentHp = FMath::Clamp(CurrentHp, 0.0f, MaxHp);
	}
}

float UCCharacterStatComponent::GetMaxHp() const
{
	return MaxHp;
}

float UCCharacterStatComponent::GetCurrentHp() const
{
	return CurrentHp;
}

void UCCharacterStatComponent::AddArmorBonusRate(float BonusRate)
{
	ArmorBonusRateSum += BonusRate;
}

void UCCharacterStatComponent::RemoveArmorBonusRate(float BonusRate)
{
	ArmorBonusRateSum -= BonusRate;
}

void UCCharacterStatComponent::AddAttackSpeedIncrease(float IncreaseRate)
{
	AttackSpeedIncreaseSum += IncreaseRate;
}

void UCCharacterStatComponent::RemoveAttackSpeedIncrease(float IncreaseRate)
{
	AttackSpeedIncreaseSum -= IncreaseRate;
}

void UCCharacterStatComponent::AddAttackPowerIncrease(float IncreaseRate)
{
	AttackPowerIncreaseSum += IncreaseRate;
}

void UCCharacterStatComponent::RemoveAttackPowerIncrease(float IncreaseRate)
{
	AttackPowerIncreaseSum -= IncreaseRate;
}

void UCCharacterStatComponent::AddBasicAttackDamageIncrease(float IncreaseRate)
{
	BasicAttackDamageIncreaseSum += IncreaseRate;
}

void UCCharacterStatComponent::RemoveBasicAttackDamageIncrease(float IncreaseRate)
{
	BasicAttackDamageIncreaseSum -= IncreaseRate;
}

void UCCharacterStatComponent::AddSkillDamageIncrease(float IncreaseRate)
{
	SkillDamageIncreaseSum += IncreaseRate;
}

void UCCharacterStatComponent::RemoveSkillDamageIncrease(float IncreaseRate)
{
	SkillDamageIncreaseSum -= IncreaseRate;
}

void UCCharacterStatComponent::AddDamageAmp(float IncreaseRate)
{
	DamageAmpSum += IncreaseRate;
}

void UCCharacterStatComponent::RemoveDamageAmp(float IncreaseRate)
{
	DamageAmpSum -= IncreaseRate;
}

void UCCharacterStatComponent::AddCritChance(float IncreaseRate)
{
	CritChance = FMath::Clamp(CritChance + IncreaseRate, 0.0f, 1.0f);
}

void UCCharacterStatComponent::RemoveCritChance(float IncreaseRate)
{
	CritChance = FMath::Clamp(CritChance - IncreaseRate, 0.0f, 1.0f);
}

void UCCharacterStatComponent::AddCritDamageIncrease(float IncreaseRate)
{
	CritDamageIncreaseSum += IncreaseRate;
}

void UCCharacterStatComponent::RemoveCritDamageIncrease(float IncreaseRate)
{
	CritDamageIncreaseSum -= IncreaseRate;
}

void UCCharacterStatComponent::AddSkillHaste(float HasteAmount)
{
	SkillHaste = FMath::Clamp(SkillHaste + HasteAmount, 0.0f, MaxSkillHaste);
}

void UCCharacterStatComponent::RemoveSkillHaste(float HasteAmount)
{
	SkillHaste = FMath::Clamp(SkillHaste - HasteAmount, 0.0f, MaxSkillHaste);
}

void UCCharacterStatComponent::Die()
{
	if (isDead)
	{
		return;
	}

	isDead = true;
	bCanBeDamaged = false;
	// TODO: Call owner character death animation or state transition.
}
