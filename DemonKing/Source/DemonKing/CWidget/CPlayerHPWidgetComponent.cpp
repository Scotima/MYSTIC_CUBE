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


	ARoguePlayerState* PS = Cast<ARoguePlayerState>(GetOwner());

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
