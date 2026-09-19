#include "DemonKing/CWidget/CPauseMenuWidget.h"
#include "Components/Button.h"
#include "DemonKing/Online/MySessionSubsystem.h"
#include "Engine/GameInstance.h"
void UCPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (LeaveButton)
	{
		LeaveButton->OnClicked.AddUniqueDynamic(this, &UCPauseMenuWidget::OnLeaveButtonClicked);
	}
}

void UCPauseMenuWidget::OnLeaveButtonClicked()
{
	UGameInstance* GI = GetGameInstance();

	if (!GI)
	{
		return;
	}

	UMySessionSubsystem* SessionSubSystem = GI->GetSubsystem<UMySessionSubsystem>();

	if (!SessionSubSystem)
	{
		return;
	}


	SessionSubSystem->ReturnToLobby();
}
