#pragma once
#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_WeaponTrailFx.generated.h"


UCLASS()
class DEMONKING_API UANS_WeaponTrailFx : public UAnimNotifyState
{
	GENERATED_BODY()


public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	virtual FString GetNotifyName_Implementation() const override;


public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "NiagaraSystem")
	TObjectPtr<class UNiagaraSystem> niagaraSystem;

	UPROPERTY()
	FName WeaponSocketName = TEXT("Knight_Weapon");

	UPROPERTY()
	TObjectPtr<class ACKnight> Knight;
	
};
