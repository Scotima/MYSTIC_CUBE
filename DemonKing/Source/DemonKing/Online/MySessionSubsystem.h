#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MySessionSubsystem.generated.h"

UENUM(BlueprintType)

enum class ESessionFlowState : uint8
{
	Idle,
	Creating,
	HostingLobby,
	Destroying,
	Error
};

class IOnlineSubsystem;


UCLASS()
class DEMONKING_API UMySessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, Category = "Online")
	bool ESureOssInterfaces();

	UFUNCTION(BlueprintCallable, Category = "Session")
	void MakeSession();

	UFUNCTION(BlueprintCallable, Category = "Session")
	void MakeSessionComplete(FName SessionName, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable, Category = "Session")
	void FindSession();

	UFUNCTION(BlueprintCallable, Category = "Session")
	void FindSessionComplete(bool bWasSuccessful);

public:
	void JoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

public:

	UPROPERTY(BlueprintReadOnly, Category = "Lobby")
	FString LobbyMapPath = TEXT("/Game/Maps/L_MainMenu");


private:
	IOnlineSessionPtr OnlineSessionInterface;

	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;

	FDelegateHandle CreateSessionCompleteDelegateHandle;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	FOnFindSessionsCompleteDelegate OnFindSessionCompleteDelegate;

	FDelegateHandle FindSessionCompleteDelegateHandle;

	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;

	FDelegateHandle JoinSessionCompleteDelegateHandle;



};
