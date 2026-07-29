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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NiagaraSystem")
	TObjectPtr<class UNiagaraSystem> niagaraSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NiagaraSystem")
	FName SocketName = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NiagaraSystem")
	FVector LocationOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NiagaraSystem")
	FRotator RotationOffset = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NiagaraSystem")
	FVector Scale = FVector(1.0f, 1.0f, 1.0f);


	UPROPERTY()
	TObjectPtr<class ACKnight> Knight;
};
