#include "DemonKing/AnimNotify/AN_SpawnBoxTrace.h"
#include "DemonKing/ActorComponent/PlayerComponent/CCharacterStatComponent.h"

void UAN_SpawnBoxTrace::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp)
	{
		return;
	}

	AActor* Owner = MeshComp->GetOwner();

	if (!Owner)
	{
		return;
	}

	UCCharacterStatComponent* SkillComponent = Owner->FindComponentByClass<UCCharacterStatComponent>();
	if (!SkillComponent)
	{
		return;
	}

	SkillComponent->DoTrace(BoxTraceData);

}
