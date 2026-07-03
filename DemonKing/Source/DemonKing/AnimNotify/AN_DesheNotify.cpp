#include "DemonKing/AnimNotify/AN_DesheNotify.h"
#include "DemonKing/CCharacter/CKnight.h"

void UAN_DesheNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    ACKnight* Knight = Cast<ACKnight>(MeshComp->GetOwner());

    if (Knight)
    {
        Knight->SkillDeshe();
    }

    else
    {
        UE_LOG(LogTemp, Warning, TEXT("AN_DesheNotify: Owner is not a CKnight."));
    }
}


