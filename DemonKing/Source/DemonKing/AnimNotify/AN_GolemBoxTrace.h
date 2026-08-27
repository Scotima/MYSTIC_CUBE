
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "DemonKing/SkillStruct/BoxTraceTypes.h"
#include "AN_GolemBoxTrace.generated.h"


UCLASS()
class DEMONKING_API UAN_GolemBoxTrace : public UAnimNotify
{
	GENERATED_BODY()

	
protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	FBoxTraceData BoxTraceData;

};
