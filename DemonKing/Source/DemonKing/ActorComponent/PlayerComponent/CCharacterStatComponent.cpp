#include "DemonKing/ActorComponent/PlayerComponent/CCharacterStatComponent.h"
#include "DemonKing/GameFlow/RoguePlayerState.h"


UCCharacterStatComponent::UCCharacterStatComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

	MaxHp = CurrentHp;
}




// Called when the game starts
void UCCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	APawn* OwnerPawn = Cast<APawn>(GetOwner());

	if (!OwnerPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UCCharacterStatComponent] :: TakeDamage !OwnerPawn"));
		return;
	}
	ARoguePlayerState* PS = Cast<ARoguePlayerState>(OwnerPawn->GetPlayerState());

	if (!PS)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UCCharacterStatComponent] :: TakeDamage !Ps"));
		return;
	}

	PS->SetPlayerState_HP(GetHP_Percent());

	
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

	APawn* OwnerPawn = Cast<APawn>(GetOwner());

	if (!OwnerPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UCCharacterStatComponent] :: TakeDamage !OwnerPawn"));
		return;
	}
	ARoguePlayerState* PS = Cast<ARoguePlayerState>(OwnerPawn->GetPlayerState());

	if (!PS)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UCCharacterStatComponent] :: TakeDamage !Ps"));
		return;
	}

	PS->SetPlayerState_HP(GetHP_Percent());

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

