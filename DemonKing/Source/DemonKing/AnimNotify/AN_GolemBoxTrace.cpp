#include "DemonKing/AnimNotify/AN_GolemBoxTrace.h"
#include "DemonKing/ActorComponent/EnemyComponent/CEnemyStatComponent.h"
void UAN_GolemBoxTrace::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!IsValid(MeshComp))
	{

		return;
	}

	AActor* Actor = MeshComp->GetOwner();

	if (!IsValid(Actor))
	{
		return;
	}


	UCEnemyStatComponent* ESC = Actor->FindComponentByClass<UCEnemyStatComponent>();

	if (!ESC)
	{
		return;
	}


	ESC->DoTrace(BoxTraceData);
}
