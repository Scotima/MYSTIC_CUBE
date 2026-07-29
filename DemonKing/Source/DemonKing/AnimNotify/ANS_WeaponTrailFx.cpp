#include "DemonKing/AnimNotify/ANS_WeaponTrailFx.h"
#include "DemonKing/CCharacter/CKnight.h"
#include "GameFramework/CharacterMovementComponent.h"

void UANS_WeaponTrailFx::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Knight = Cast<ACKnight>(MeshComp->GetOwner());

	if (Knight)
	{
		Knight->GetCharacterMovement()->StopMovementImmediately();
		Knight->GetCharacterMovement()->MaxWalkSpeed = 0;
		Knight->StartNiagaraImpact(niagaraSystem, WeaponSocketName);
	}


}

void UANS_WeaponTrailFx::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

	Knight = Cast<ACKnight>(MeshComp->GetOwner());

	if (Knight)
	{
		Knight->EndNiagaraImpact();
		Knight->GetCharacterMovement()->MaxWalkSpeed = 600;
	}

}

FString UANS_WeaponTrailFx::GetNotifyName_Implementation() const
{
	return TEXT("WeaponTrailFx");
}
