#include "DemonKing/FoxGirl/CanComboInputNotify.h"
#include "DemonKing/FoxGirl/FoxSkillMouseLeftComponent.h"
void UCanComboInputNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{

	Super::Notify(MeshComp, Animation, EventReference);
	if (!IsValid(MeshComp))
	{
		return;
	}
	AActor* OwnerCharacter = MeshComp->GetOwner();
	if (!IsValid(OwnerCharacter))
	{
		return;
	}
	UFoxSkillMouseLeftComponent* SkillComponent =
		OwnerCharacter->FindComponentByClass<UFoxSkillMouseLeftComponent>();
	if (!IsValid(SkillComponent))
	{
		return;
	}
	SkillComponent->SetCanInputCombo(true);

}
