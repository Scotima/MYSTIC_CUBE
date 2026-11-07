#include "DemonKing/AnimNotify/AnimNotify_EnableMoveCancel.h"
#include "DemonKing/CCharacter/RogueCharacterBase.h"
void UAnimNotify_EnableMoveCancel::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	ARogueCharacterBase* RCB = Cast<ARogueCharacterBase>(MeshComp->GetOwner());

	if (!IsValid(RCB))
	{
		return;
	}

	RCB->HasMoveInput();

	if (RCB->HasMoveInput())
	{
		RCB->MulticastCancelAttackForMovement(); // 공격 모션을 취소하고 이동으로 전환

	}



}
