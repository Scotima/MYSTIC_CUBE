#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DemonKing/SkillStruct/BoxTraceTypes.h"
#include "CEnemyStatComponent.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnEnemyDied);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnEmenyHpChanged, float);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DEMONKING_API UCEnemyStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UCEnemyStatComponent();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void AttackPlayer(AActor* HicActor);
	void TakeDamage(float PlayerPower,float DefensePenetration,float DefenseIgnoreRate); // was Attacked by Player
	void DoTrace(const FBoxTraceData& BoxTraceData); //Call AnimNotify;
	void Die();

	UFUNCTION(Server, Reliable)
	void ServerAttackPlayer(AActor* HitActor);

	

	UFUNCTION()
	void OnRep_Current();

	FORCEINLINE void SetMaxHp(float Hp) { MaxHp = Hp; }
	FORCEINLINE void SetAttackPower(float Power) { AttackPower = Power; }
	FORCEINLINE float GetHealthPercent() { return MaxHp > 0.0f ? CurrentHp / MaxHp : 0.0f; }

	void DropItem();

public:
	FOnEnemyDied OnEnemyDied;
	FOnEmenyHpChanged OnEmenyHpChanged;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<class ACItemBase> Item;

private:
	float MaxHp;

	UPROPERTY(ReplicatedUsing = OnRep_Current)
	float CurrentHp;

	float AttackPower;
	float MoveSpeed;
	

	bool isDead;
	bool bCanBeDamaged;

	float Defense;



		
};
