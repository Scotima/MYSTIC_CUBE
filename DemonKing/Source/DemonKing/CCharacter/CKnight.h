#pragma once
#include "CoreMinimal.h"
#include "DemonKing/CCharacter/RogueCharacterBase.h"
#include "CKnight.generated.h"


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


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Socket")
	TObjectPtr<class UStaticMeshComponent> WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Socket")
	FName WeaponSocketName = TEXT("hand_r_Socket");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Niagara")
	TObjectPtr<class UNiagaraComponent> NiagaraComponent;
	
};
