#include "DemonKing/AIController/CStoneGolemAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

ACStoneGolemAIController::ACStoneGolemAIController()
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
}

void ACStoneGolemAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (!BehaviorTree || !BehaviorTree->BlackboardAsset)
	{
		return;
	}

	UBlackboardComponent* BlackboardPtr = nullptr;

	if (!UseBlackboard(BehaviorTree->BlackboardAsset, BlackboardPtr))
	{
		return;
	}

	BlackboardComp = BlackboardPtr;
	SetInitialTarget();
	RunBehaviorTree(BehaviorTree);
}

void ACStoneGolemAIController::SetInitialTarget()
{
	if (!BlackboardComp)
	{
		return;

	}

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (!PlayerPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("NONONO"));
		return;

	}

	BlackboardComp->SetValueAsObject(TEXT("TargetActor"), PlayerPawn);
}
