#include "DemonKing/FoxGirl/RogueCharacterMovementComponent.h"
#include "GameFramework/Character.h"

float URogueCharacterMovementComponent::SlideAlongSurface(const FVector& Delta, float Time, const FVector& Normal, FHitResult& Hit, bool bHandleImpact)
{
	const ACharacter* OtherCharacter = Cast<ACharacter>(Hit.GetActor());

	if (CharacterOwner && CharacterOwner->IsPlayingRootMotion() && OtherCharacter && OtherCharacter != CharacterOwner)
	{
		return 0.f; // 조건이 맞으면 슬라이드하지 않고 0을 반환하여 충돌을 무시합니다.
	}

	return Super::SlideAlongSurface(Delta, Time, Normal, Hit, bHandleImpact);
}
