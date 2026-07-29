#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CStoneGolemAIController.generated.h"


UCLASS()
class DEMONKING_API ACStoneGolemAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ACStoneGolemAIController();
	
protected:
	virtual void OnPossess(APawn* InPawn) override;


private:
	void SetInitialTarget();

private:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	TObjectPtr<UBlackboardComponent> BlackboardComp;
	
};
