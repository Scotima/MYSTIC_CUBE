#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

class ARoguePlayerState;

UCLASS()
class DEMONKING_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()

	

protected:
	virtual void NativeDestruct() override;

public:
	UFUNCTION(BlueprintCallable, Category = "State")
	void SetUpEntry(ARoguePlayerState* playerstate);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void HandleNicknameChanged(const FString& Nick);

private:
	void RefreshNickName();





public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Text_Nickname;

	UPROPERTY()
	TObjectPtr<ARoguePlayerState> cachedPlayerState;
	
};
