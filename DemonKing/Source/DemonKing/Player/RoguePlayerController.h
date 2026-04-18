#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RoguePlayerController.generated.h"

UENUM(BlueprintType)

enum class ETypeControll : uint8
{
	Main,
	Lobby,
	Game
};


class UUserWidget;

UCLASS()
class DEMONKING_API ARoguePlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	ARoguePlayerController();

	virtual void BeginPlay()override;
	virtual void Tick(float DeltaTime) override;

	//virtual void EndPlay() override;


public:
	//https://parkcy723.tistory.com/104 ÄÚµå

	UFUNCTION()
	void LookMouseCursor();



	//-------------------------//

	UFUNCTION()
	void AfterChangeWorldMap(UWorld* LoadedWorld);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_RequestStartRun();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_SetLobbyNickName(const FString& newname);

private:
	virtual void SetupInputComponent() override;


private:
	void MoveForward(float value);
	void MoveRight(float value);

	void OnJumpPressed();
	void OnJumpReleased();

	void OnMouseLeftClick();


private:

	UFUNCTION()
	void ApplyMode(ETypeControll controll);

	UFUNCTION()
	void UpdateWorldName();

	UFUNCTION()
	FString GetPlayerNickName();

	UFUNCTION()
	void SubmitMyLobbyNickName();

	UFUNCTION()
	void BackToLobby(bool bWasSuccessful);

public:

private:

	UPROPERTY()
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UPROPERTY()
	TSubclassOf<UUserWidget> StartGameWidgetClass;

	UPROPERTY()
	UUserWidget* MainMenuWidget;

	UPROPERTY()
	UUserWidget* StartGameWidget;

	UPROPERTY()
	TObjectPtr<class ARogueCharacterBase> characterBase;


private:
	ETypeControll Mode = ETypeControll::Main;	
	
};
