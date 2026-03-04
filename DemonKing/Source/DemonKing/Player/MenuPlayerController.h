#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MenuPlayerController.generated.h"

class UMySessionSubsystem;
class UUserWidget;

UCLASS()
class DEMONKING_API AMenuPlayerController : public APlayerController
{
	GENERATED_BODY()


public:
	AMenuPlayerController();

public:
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_RequestStartRun();

public:
	UFUNCTION(BlueprintCallable)
	void MakeMultiRoom();

	UFUNCTION(BlueprintCallable)
	void FindMultiRoom();



protected:
	virtual void BeginPlay() override;

	void ShowMenuWidget();


private:
	UMySessionSubsystem* GetMSS();

public:
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> TestWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UPROPERTY()
	UUserWidget* TestWidget;

	UPROPERTY()
	UUserWidget* MainMenuWidget;

private:
	UPROPERTY()
	UMySessionSubsystem* MSS;


	UPROPERTY()
	bool bWasSuccessful = false;
};
