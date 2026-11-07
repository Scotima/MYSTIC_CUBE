
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Knight_Strafe.generated.h"

UCLASS()
class DEMONKING_API UBTTask_Knight_Strafe : public UBTTaskNode
{
	GENERATED_BODY()


protected:
	virtual EBTNodeResult::Type ExecuteTask(
		UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


private:
	FVector CaculateLookYaw(const FVector& BossLocation,
		const FVector& TargetLocation, float AngleDegrees) const;
	
};
