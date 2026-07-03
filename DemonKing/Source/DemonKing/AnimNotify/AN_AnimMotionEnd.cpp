#include "DemonKing/AnimNotify/AN_AnimMotionEnd.h"
#include "AN_AnimMotionEnd.h"
#include "DemonKing/SkillComponent/CKnightSkillComponent.h"
#include "DemonKing/CCharacter/CKnight.h"
#include "DemonKing/Player/RoguePlayerController.h"

void UAN_AnimMotionEnd::Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation)
{
    UCKnightSkillComponent* KnightSkillComponent = MeshComp->GetOwner()->FindComponentByClass<UCKnightSkillComponent>();

    if (KnightSkillComponent)
    {
        KnightSkillComponent->SetMotionEnd(true);

        ACKnight* OwnerCharacter = Cast<ACKnight>(MeshComp->GetOwner());

        if (OwnerCharacter)
        {
            OwnerCharacter->CheckingAttackPose();
            ARoguePlayerController *Controller = Cast<ARoguePlayerController>(OwnerCharacter->GetController());
            if (Controller)
            {
                Controller->SetIgnoreMoveInput(false);
                Controller->SetCanInput(true);

                
            }
        }
    }
}