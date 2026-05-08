#include "DemonKing/AnimNotify/CSpawnNiagaraSystemNotify.h"
#include "DemonKing/CCharacter/CKnight.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCSpawnNiagaraSystemNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Knight = Cast<ACKnight>(MeshComp->GetOwner());

	if (Knight)
	{
		Knight->GetCharacterMovement()->StopMovementImmediately();
		Knight->GetCharacterMovement()->MaxWalkSpeed = 0;

		if (niagaraSystem)
		{
			Knight->SpawnNiagaraSystem(niagaraSystem, SocketName, LocationOffset, RotationOffset, Scale);
			
		}
	}

}

void UCSpawnNiagaraSystemNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Knight = Cast<ACKnight>(MeshComp->GetOwner());
	
	if (Knight)
	{
		Knight->GetCharacterMovement()->MaxWalkSpeed = 300;
	}
}
