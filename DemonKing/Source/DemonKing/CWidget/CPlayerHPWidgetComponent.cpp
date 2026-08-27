#include "DemonKing/CWidget/CPlayerHPWidgetComponent.h"
#include "DemonKing/GameFlow/RoguePlayerState.h"
#include "DemonKing/CWidget/CPlayerHPWidget.h"


void UCPlayerHPWidgetComponent::BeginPlay()
{

	Super::BeginPlay();
	InitWidget();

	PlayerHPWidget = Cast<UCPlayerHPWidget>(GetUserWidgetObject());
	if (!PlayerHPWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UCPlayerHPWidgetComponent]"
			":: BeginPlay !PlayerHPWidget"));
		return;
	}

	APawn* OwnerPawn = Cast<APawn>(GetOwner());

	if (!IsValid(OwnerPawn))
	{
		return;
	}



	ARoguePlayerState* PS = Cast<ARoguePlayerState>(OwnerPawn->GetPlayerState());

	if (!PS)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UCPlayerHPWidgetComonent]"
			":: BeginPlay !PS"));
		return;
	}

	PS->OnPlayerHpChanged.AddUObject(this,
		&UCPlayerHPWidgetComponent::UpdateHealthComponent);


	PlayerHPWidget->UpdatePlayerHealth(PS->GetPlayerState_HP());





}

void UCPlayerHPWidgetComponent::UpdateHealthComponent(float CurrentHp)
{
	if (!IsValid(PlayerHPWidget))
	{
		UE_LOG(LogTemp, Warning, TEXT("[UCPlayerHPWidgetComponent]"
			"::UpdateHealthComponent !IsValid PlayerHPWidget"));
		return;
	}

	PlayerHPWidget->UpdatePlayerHealth(CurrentHp);
}
