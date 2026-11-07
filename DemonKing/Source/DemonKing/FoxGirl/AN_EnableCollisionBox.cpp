#include "DemonKing/FoxGirl/AN_EnableCollisionBox.h"
#include "DemonKing/FoxGirl/CFoxGirl.h"
#include "DemonKing/FoxGirl/CKatanaComponent.h"
void UAN_EnableCollisionBox::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{


	
	ACFoxGirl* OwnerCharacter = Cast<ACFoxGirl>(MeshComp->GetOwner());

	if (!IsValid(OwnerCharacter))
	{
		return;
	}

	ACKatanaComponent* KatanaComponent = OwnerCharacter->GetKatanaComponent();

	if (IsValid(KatanaComponent))
	{
		KatanaComponent->SetAttackCollisionEnabled(bEnableCollision);
	}	
}
