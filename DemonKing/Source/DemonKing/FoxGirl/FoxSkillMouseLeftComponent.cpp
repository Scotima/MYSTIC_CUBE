#include "DemonKing/FoxGirl/FoxSkillMouseLeftComponent.h"
#include "FoxSkillMouseLeftComponent.h"
#include "DemonKing/CCharacter/RogueCharacterBase.h"

void UFoxSkillMouseLeftComponent::PlaySkillMontage()
{

   

    if(ComboMontages.IsEmpty() || !ComboMontages.IsValidIndex(ComboIndex))
    {
        UE_LOG(LogTemp, Warning, TEXT("IsEmpty?"));
        return;
    }



    if(CanInputCombo)
    {
        
        ComboIndex = (ComboIndex + 1) % ComboMontages.Num();
        CanInputCombo = false;
    }

    SkillMontage = ComboMontages[ComboIndex];
    Super::PlaySkillMontage();
    UE_LOG(LogTemp, Warning, TEXT("PlaySkillMontage"));


}