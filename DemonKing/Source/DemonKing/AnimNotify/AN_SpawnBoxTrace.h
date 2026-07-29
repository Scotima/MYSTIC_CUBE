#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "DemonKing/SkillStruct/BoxTraceTypes.h"
#include "AN_SpawnBoxTrace.generated.h"

UCLASS()
class DEMONKING_API UAN_SpawnBoxTrace : public UAnimNotify
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoxTraceTypes")
	FBoxTraceData BoxTraceData;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
