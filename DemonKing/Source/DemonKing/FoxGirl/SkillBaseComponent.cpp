#include "DemonKing/FoxGirl/SkillBaseComponent.h"
#include "DemonKing/CCharacter/RogueCharacterBase.h"
#include "GameFramework/Character.h"

USkillBaseComponent::USkillBaseComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

	
}



void USkillBaseComponent::BeginPlay()
{
	Super::BeginPlay();

	

	OwnerCharacter = Cast<ARogueCharacterBase>(GetOwner());
	
	
}



void USkillBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

void USkillBaseComponent::PlaySkillMontage()
{
	if (!IsValid(SkillMontage))
	{
		return;
	}

	OwnerCharacter->MulticastPlaySkillMontage(SkillMontage);
}

