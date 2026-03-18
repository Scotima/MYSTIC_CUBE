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
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class ULobbyWidget> lobbyentryclass;



protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UVerticalBox> playerList;
	
};
