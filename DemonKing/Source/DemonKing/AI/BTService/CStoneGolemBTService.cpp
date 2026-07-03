#include "DemonKing/AI/BTService/CStoneGolemBTService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UCStoneGolemBTService::UCStoneGolemBTService()
{
	NodeName = TEXT("Update Target Actor");

	Interval = 0.2f;
	RandomDeviation = 0.0f;
	bNotifyTick = true;
}

void UCStoneGolemBTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return;
	}

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(OwnerComp.GetWorld(), 0);
	if (!PlayerPawn)
	{
		BlackboardComp->ClearValue(TEXT("TargetActor"));
		return;
	}

	BlackboardComp->SetValueAsObject(TEXT("TargetActor"), PlayerPawn);


}
