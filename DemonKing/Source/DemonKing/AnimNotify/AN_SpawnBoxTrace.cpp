#include "DemonKing/AnimNotify/AN_SpawnBoxTrace.h"
#include "DemonKing/SkillComponent/CKnightSkillComponent.h"

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

	UCKnightSkillComponent* SkillComponent = Owner->FindComponentByClass<UCKnightSkillComponent>();
	if (!SkillComponent)
	{
		return;
	}

	SkillComponent->DoTrace(BoxTraceData);

}
