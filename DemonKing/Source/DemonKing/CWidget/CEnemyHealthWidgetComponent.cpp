#include "DemonKing/CWidget/CEnemyHealthWidgetComponent.h"
#include "DemonKing/ActorComponent/EnemyComponent/CEnemyStatComponent.h"
#include "DemonKing/CWidget/CEnemyHealthWidget.h"
void UCEnemyHealthWidgetComponent::BeginPlay()
{
	Super::BeginPlay();
	InitWidget();

	EnemyHealthWidget = Cast<UCEnemyHealthWidget>(GetUserWidgetObject());



	if (!IsValid(EnemyHealthWidget))
	{
		UE_LOG(LogTemp, Warning, TEXT("[UCEnemyHealthWidgetComponent]::BeginPlay !IsValid EnemyHealthWidget"));
		return;
	}






	UCEnemyStatComponent* EST = GetOwner()->FindComponentByClass<UCEnemyStatComponent>();

	if (!EST)
	{
		//UE_LOG(LogTemp, Warning, TEXT("[UCEnemyHealthWidgetComponent] BeginPlay !EST"));
		return;
	}

	EST->OnEmenyHpChanged.AddUObject(this, &UCEnemyHealthWidgetComponent::UpdateHealth);

	EnemyHealthWidget->UpdateHealthWidget(EST->GetHealthPercent());
}

void UCEnemyHealthWidgetComponent::UpdateHealth(float Hp)
{
	
	if (!IsValid(EnemyHealthWidget))
	{
		UE_LOG(LogTemp, Warning, TEXT("[UCEnemyHealthWidgetComponent] :: UpdateHealth !IsValid EnemyHealthWidget"));
		return;
	}

	EnemyHealthWidget->UpdateHealthWidget(Hp);

}
