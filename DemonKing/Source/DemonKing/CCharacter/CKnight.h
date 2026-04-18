#pragma once
#include "CoreMinimal.h"
#include "DemonKing/CCharacter/RogueCharacterBase.h"
#include "CKnight.generated.h"

class UNiagaraComponent;


UCLASS()
class DEMONKING_API ACKnight : public ARogueCharacterBase
{
	GENERATED_BODY()


public:
	ACKnight();
	
	UFUNCTION(BlueprintCallable)
	void StartNiagaraImpact(class UNiagaraSystem* niagaraSystem, FName Socketname);

	UFUNCTION(BlueprintCallable)
	void EndNiagaraImpact();

public:
	virtual void InputSkillLeftMouse() override;
	
	

public:
	UNiagaraComponent* SpawnNiagaraSystem(class UNiagaraSystem* niagarasystem);
	float PlaySkillMotion(class UAnimMontage* animmontage, float PlayRate = 1.0f);


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Socket")
	TObjectPtr<class UStaticMeshComponent> WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Socket")
	FName WeaponSocketName = TEXT("hand_r_Socket");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Niagara")
	TObjectPtr<UNiagaraComponent> NiagaraComponent;
	
};
