#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CSpawnNiagaraSystemNotify.generated.h"


UCLASS()
class DEMONKING_API UCSpawnNiagaraSystemNotify : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;


public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "NiagaraSystem")
	TObjectPtr<class UNiagaraSystem> niagaraSystem;

	UPROPERTY()
	TObjectPtr<class ACKnight> Knight;
};
