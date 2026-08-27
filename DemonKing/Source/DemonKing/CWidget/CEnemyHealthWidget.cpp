#include "DemonKing/CWidget/CEnemyHealthWidget.h"
#include "Components/ProgressBar.h"

void UCEnemyHealthWidget::UpdateHealthWidget(float HealthPercent)
{
	if (!IsValid(HealthProgressBar))
	{
		UE_LOG(LogTemp, Warning, TEXT("[UCEnemyHealthWidget]::UpdateHealthWidget !IsValid HealthProgressBar"));
		return;
	}

	HealthProgressBar->SetPercent(FMath::Clamp(HealthPercent, 0.0f, 1.0f));
}
