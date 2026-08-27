#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DemonKing/SkillStruct/CharacterSkillStruct.h"
#include "DemonKing/SkillStruct/BoxTraceTypes.h"
#include "CKnightSkillComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DEMONKING_API UCKnightSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCKnightSkillComponent();	

	UFUNCTION(BlueprintCallable, Category = "Skill")
	void UseSkill(int SkillID, int32 ComboIndex);

public:
	FORCEINLINE void SetMotionEnd(bool a) {MotionEnd = a;}
	FORCEINLINE bool GetMotionEnd() { return MotionEnd; }



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void DoTrace(const FBoxTraceData& BoxTraceData);

	FORCEINLINE void SetCanInput(bool a) { bCanInput = a; }
	FORCEINLINE bool GetCanInput() { return bCanInput; }

private:
	FCharacterSkillStruct* GetSkillDataTable(FName rowname);

	bool CoolDownSystem(int SkillID);

protected:

	UPROPERTY()
	TObjectPtr<class ACKnight> OwnerCharacter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataTable")
	TObjectPtr<UDataTable> SkillDataTable;

private:
	TMap<int, float> SkillCoolTimeMap;

	float CurrentSkillDamageCoefficient = 1.0f;
	bool bCurrentSkillIsBasicAttack = false;



private:
	FTimerHandle timerhandle;

	bool bUseSkill = true;

	int BeforeSKillId = 0;

	bool MotionEnd = true;

	bool bCanInput = true;
		
};
