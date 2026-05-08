#include "DemonKing/AnimNotify/ANS_ComboInputWindow.h"
#include "DemonKing/CCharacter/CKnight.h"

void UANS_ComboInputWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	knight = Cast<ACKnight>(MeshComp->GetOwner());

	if (knight)
	{
		
		knight->OpenComboInput();
		
	}
}

void UANS_ComboInputWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

	knight = Cast<ACKnight>(MeshComp->GetOwner());

	if (knight)
	{
		knight->CloseComboInput();
	}


}
