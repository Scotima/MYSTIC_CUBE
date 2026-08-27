#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DemonKing/SkillStruct/BoxTraceTypes.h"
#include "CEnemyStatComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DEMONKING_API UCEnemyStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCEnemyStatComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AttackPlayer(AActor* HitActor);

	UFUNCTION(BlueprintCallable, Category = "Stat")
	void TakeDamage(float IncomingDamage);

	void TakeDamage(float IncomingDamage, float DefensePenetration, float DefenseIgnoreRate);
	void DoTrace(const FBoxTraceData& BoxTraceData);
	void Die();

	FORCEINLINE void SetMaxHp(float Hp) { MaxHp = FMath::Max(0.0f, Hp); CurrentHp = MaxHp; }
	FORCEINLINE void SetAttackPower(float Power) { AttackPower = FMath::Max(0.0f, Power); }

	UFUNCTION(BlueprintPure, Category = "Stat")
	float CalculateFinalDamageTaken(float IncomingDamage, float DefensePenetration, float DefenseIgnoreRate) const;

	UFUNCTION(BlueprintCallable, Category = "Stat|Modifier")
	void AddDamageTakenAmp(float IncreaseRate);

	UFUNCTION(BlueprintCallable, Category = "Stat|Modifier")
	void RemoveDamageTakenAmp(float IncreaseRate);

	void DropItem();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<class ACItemBase> Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat|Base")
	float MaxHp = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat|Base")
	float CurrentHp = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat|Base")
	float AttackPower = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat|Base")
	float MoveSpeed = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat|Base")
	float Defense = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat|Modifier")
	float DamageTakenAmpSum = 0.0f;

private:
	bool isDead = false;
	bool bCanBeDamaged = true;

	static constexpr float ArmorK = 200.0f;
	static constexpr float MaxDamageReduction = 0.8f;
};
