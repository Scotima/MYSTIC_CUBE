#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CStoneGolemBTService.generated.h"

UCLASS()
class DEMONKING_API UCStoneGolemBTService : public UBTService
{
	GENERATED_BODY()
	
public:
	UCStoneGolemBTService();


protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
