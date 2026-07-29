#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DemonKing/SkillStruct/BoxTraceTypes.h"
#include "CEnemyStatComponent.generated.h"


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

	void AttackPlayer(AActor* HicActor);
	void TakeDamage(float PlayerPower,float DefensePenetration,float DefenseIgnoreRate); // was Attacked by Player
	void DoTrace(const FBoxTraceData& BoxTraceData); //Call AnimNotify;
	void Die();

	FORCEINLINE void SetMaxHp(float Hp) { MaxHp = Hp; }
	FORCEINLINE void SetAttackPower(float Power) { AttackPower = Power; }

	void DropItem();


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<class ACItemBase> Item;

private:
	float MaxHp;
	float CurrentHp;

	float AttackPower;
	float MoveSpeed;
	

	bool isDead;
	bool bCanBeDamaged;

	float Defense;



		
};
