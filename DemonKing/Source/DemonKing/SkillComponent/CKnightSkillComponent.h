#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DemonKing/SkillStruct/CharacterSkillStruct.h"
#include "CKnightSkillComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DEMONKING_API UCKnightSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCKnightSkillComponent();	

	UFUNCTION(BlueprintCallable, Category = "Skill")
	void UseSkill(FName skillname);



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	FCharacterSkillStruct* GetSkillDataTable(FName rowname);

	void CoolDownSystem();

protected:

	UPROPERTY()
	TObjectPtr<class ACKnight> OwnerCharacter;

	UPROPERTY()
	TObjectPtr<UDataTable> SkillDataTable;


private:
	FTimerHandle timerhandle;

	bool bUseSkill = true;
		
};
