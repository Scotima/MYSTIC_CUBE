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

	UFUNCTION(BlueprintCallable, Category = "Session")
	void OnSessionUserInviteAccept();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowInviteUI();


	



public:
	void JoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void SetStartSessionCompleteSuccessful(bool bwasSuccessful) { StartSessionCompleteSuccessful = bwasSuccessful; }

	bool GetStartSessionCompleteSuccessful() { return StartSessionCompleteSuccessful; }

	void OnSessionUserInviteAccepted(const bool bWasSuccess, const int32 ControllerId, FUniqueNetIdPtr UserId, const FOnlineSessionSearchResult& InviteResult);


private:
	void StartSession();
	void StartSessionComplete(FName SessionName, bool bwasSuccessful);

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

	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;

	FDelegateHandle StartSessionCompleteDelegateHandle;

	FOnSessionUserInviteAcceptedDelegate OnSessionUserInviteAcceptedDelegate;
	FDelegateHandle SessionUserInviteAcceptedDelegateHandle;


	class IOnlineSubsystem* onlinesubsystem = nullptr;



private:
	bool StartSessionCompleteSuccessful = false;



};
