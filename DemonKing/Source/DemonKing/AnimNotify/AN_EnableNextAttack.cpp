// Fill out your copyright notice in the Description page of Project Settings.
#include "DemonKing/AnimNotify/AN_EnableNextAttack.h"
#include "DemonKing/CCharacter/RogueCharacterBase.h"
void UAN_EnableNextAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	ARogueCharacterBase* RogueCharacter = Cast<ARogueCharacterBase>(MeshComp->GetOwner());

	if(!IsValid(RogueCharacter))
	{
		return;
	}

	RogueCharacter->SetIsAttacking(false);
}
