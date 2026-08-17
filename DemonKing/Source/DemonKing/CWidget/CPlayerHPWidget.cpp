#include "DemonKing/CWidget/CPlayerHPWidget.h"
#include "Components/ProgressBar.h"

void UCPlayerHPWidget::UpdatePlayerHealth(float CurrentHp)
{
	if (!IsValid(HealthBar))
	{
		UE_LOG(LogTemp, Warning, TEXT("[UCPlayerHPWidget]::UpdatePlayerHealth"));
		return;
	}

	HealthBar->SetPercent(FMath::Clamp(CurrentHp, 0.0f, 1.0f));

}
