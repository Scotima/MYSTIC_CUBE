#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyMainWidget.generated.h"


UCLASS()
class DEMONKING_API ULobbyMainWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable)
	void RefreshPlayerList();

protected:
	UFUNCTION()
	void HandleReturnToLobby();

	UFUNCTION()
	void ShowInviteWidget();


private:
	class UMySessionSubsystem* GetMYSS();


	

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class ULobbyWidget> lobbyentryclass;



protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UVerticalBox> playerList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BackButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> btn_InviteButton;

private:
	UMySessionSubsystem* mySubsystem;
};
