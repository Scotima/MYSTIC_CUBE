#include "DemonKing/AnimNotify/AN_AnimMotionEnd.h"
#include "AN_AnimMotionEnd.h"
#include "DemonKing/SkillComponent/CKnightSkillComponent.h"

void UAN_AnimMotionEnd::Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation)
{
    UCKnightSkillComponent* KnightSkillComponent = MeshComp->GetOwner()->FindComponentByClass<UCKnightSkillComponent>();

    if (KnightSkillComponent)
    {
        KnightSkillComponent->SetMotionEnd(true);
    }
}