#include "DemonKing/ActorComponent/PlayerComponent/CCharacterStatComponent.h"


UCCharacterStatComponent::UCCharacterStatComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

	MaxHp = CurrentHp;
}


// Called when the game starts
void UCCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


// Called every frame
void UCCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

void UCCharacterStatComponent::TakeDamage(float MonsterPower)
{
	if (CurrentHp <= 0)
	{
		return;
	}

	CurrentHp -= MonsterPower; // this is not finalized yet. This is temporary


	if (CurrentHp <= 0.0f)
	{
		Die();
	}

	//방여력이나 체력을 곱하고 더해서 뭔갈 하겠지?
}

void UCCharacterStatComponent::Die()
{
	//Call function PlayerCharacter->PlayMontage[Death Animation]
}

