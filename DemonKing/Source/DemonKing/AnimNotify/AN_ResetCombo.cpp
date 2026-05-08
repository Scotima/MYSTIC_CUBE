#include "DemonKing/AnimNotify/AN_ResetCombo.h"
#include "DemonKing/CCharacter/CKnight.h"

void UAN_ResetCombo::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    ACKnight* knight = Cast<ACKnight>(MeshComp->GetOwner());
    if(knight)
    {
        knight->ResetCombo();
    }
}

