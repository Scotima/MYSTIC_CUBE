#include "DemonKing/Online/MySessionSubsystem.h"
#include "OnlineSubsystem.h"
#include "Templates/SharedPointer.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineExternalUIInterface.h"


static const FName KEY_MatchType(TEXT("MatchType"));
static const FName KEY_GameTag(TEXT("GameTag"));

bool UMySessionSubsystem::ESureOssInterfaces()
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get(); //온라인 서버 만들 수 있는 권한을 가지고 있나?

	if (!OnlineSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnlineSubsystem is nullptr"));
		return false;
	}

	OnlineSessionInterface = OnlineSubsystem->GetSessionInterface();
	if (!OnlineSessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("OnlineSessionInterface is nullptr"));
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("OnlineSessionInterface : %s"), *OnlineSubsystem->GetSubsystemName().ToString());
	return true;

}

void UMySessionSubsystem::MakeSession()
{
	if (!ESureOssInterfaces())
	{
		UE_LOG(LogTemp, Warning, TEXT("EsureOSSInterfaces() is return false"));
		return;
	}
	//https://hvvan.tistory.com/274 코드.
	const auto ExisitingSession = OnlineSessionInterface->GetNamedSession(NAME_GameSession);

	if (ExisitingSession != nullptr)
	{
		OnlineSessionInterface->DestroySession(NAME_GameSession);
		UE_LOG(LogTemp, Warning, TEXT("ExisitingSession Found. Destroy Requested. TryAgain"));
		return;
	}
	

	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UMySessionSubsystem::MakeSessionComplete);


	CreateSessionCompleteDelegateHandle = OnlineSessionInterface->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

	//세션 세팅
	TSharedPtr<FOnlineSessionSettings> SessionSettings = MakeShareable(new FOnlineSessionSettings());

	SessionSettings->bIsLANMatch = false; //null 테스트를 위해서.
	SessionSettings->NumPublicConnections = 2;
	SessionSettings->bAllowJoinInProgress = true; // 세션 실행중 참여 여부.
	SessionSettings->bAllowJoinViaPresence = true; 
	SessionSettings->bShouldAdvertise = true; // 스팀을 통해 세션 알림.
	SessionSettings->bUsesPresence = true;
	SessionSettings->bUseLobbiesIfAvailable = true; //로비 사용 여부.
	SessionSettings->BuildUniqueId = GetBuildUniqueId();
	SessionSettings->Set(KEY_MatchType, FString("FreeForAll"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionSettings->Set(KEY_GameTag, FString("DemonKing"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	//Session 생성 요청.
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();

	if (!LocalPlayer || !LocalPlayer->GetPreferredUniqueNetId().IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("LocalPlayer or UniqueNetId invalid. cannotCreateSession"));
		OnlineSessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
		return;
	}
	const bool bCreateStarted = 
	OnlineSessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *SessionSettings);

}

void UMySessionSubsystem::MakeSessionComplete(FName SessionName, bool bWasSuccessful)
{

	if (OnlineSessionInterface.IsValid())
	{
		OnlineSessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	}
	
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Session success"));
		UWorld* World = GetWorld();

		if (!World)
		{
			UE_LOG(LogTemp, Warning, TEXT("Not a server. Skip ServerTravel."));
			return;
		}

		StartSession();
	
		//World->ServerTravel(TEXT("/Game/Maps/TESTMAP?listen?port=7777"), false);

		
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Session failed"));
	}
}

void UMySessionSubsystem::StartSession()
{
	if (!ESureOssInterfaces())
	{
		UE_LOG(LogTemp, Warning, TEXT("EsureOSSInterfaces() is return false"));
		return;
	}

	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &UMySessionSubsystem::StartSessionComplete);
	StartSessionCompleteDelegateHandle = OnlineSessionInterface->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);

	const bool bStartRequested = OnlineSessionInterface->StartSession(NAME_GameSession);

	UE_LOG(LogTemp, Warning, TEXT("StartSession Requested: %s"), bStartRequested ? TEXT("True") : TEXT("false"));

	if (!bStartRequested)
	{
		OnlineSessionInterface->ClearOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegateHandle);
	}

}


void UMySessionSubsystem::StartSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (!OnlineSessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("StartSession: OnlineSessionInterface is nullptr"));
		return;
	}

	OnlineSessionInterface->ClearOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegateHandle);

	UE_LOG(LogTemp, Warning, TEXT("StartSessionComplete: %s (Session %s)"), bWasSuccessful ? TEXT("Success") : TEXT("Fail"), *SessionName.ToString());

	if (!bWasSuccessful)
	{
		return;
	}

	SetStartSessionCompleteSuccessful(bWasSuccessful);


	OnlineSessionInterface->DumpSessionState();// ✅ (추가) 여기서 세션 상태 덤프 찍어서 Pending/RegisteredPlayers 확인

	UWorld* world = GetWorld();

	if (!world)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartSessionComplete: World null"));
		return;
	}

	world->ServerTravel(TEXT("/Game/Maps/L_Lobby?listen"), false);
}

//https://zeniff.tistory.com/23 코드.

void UMySessionSubsystem::FindSession()
{

	if (!ESureOssInterfaces())
	{
		UE_LOG(LogTemp, Warning, TEXT("EsureOSSInterfaces() is return false"));
		return;
	}

	if (!OnlineSessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("GameSession Interface is invailed"));
		return;
	}


	const ULocalPlayer* LocalPlayer = GetWorld() ? GetWorld()->GetFirstLocalPlayerFromController() : nullptr;

	if (!LocalPlayer || !LocalPlayer->GetPreferredUniqueNetId().IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("LocalPlayer or UniqueNetId invalid in FindSession"));
		return;
	}

	OnFindSessionCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &UMySessionSubsystem::FindSessionComplete);
	FindSessionCompleteDelegateHandle = OnlineSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionCompleteDelegate);

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->MaxSearchResults = 100;
	SessionSearch->bIsLanQuery = false;
	//SessionSearch->QuerySettings.Set(FName("SEARCH_PRESENCE"), true, EOnlineComparisonOp::Equals);
	SessionSearch->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
	SessionSearch->QuerySettings.Set(KEY_MatchType, FString("FreeForAll"), EOnlineComparisonOp::Equals);
	SessionSearch->QuerySettings.Set(KEY_GameTag, FString("DemonKing"), EOnlineComparisonOp::Equals);


	const bool bFindStarted = OnlineSessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef());

	UE_LOG(LogTemp, Warning, TEXT("Find Session Started: %s"), bFindStarted ? TEXT("true") : TEXT("False"));

	if (!bFindStarted)
	{
		OnlineSessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionCompleteDelegateHandle);
	}

}

void UMySessionSubsystem::FindSessionComplete(bool bWasSuccessful)
{

	if (OnlineSessionInterface.IsValid())
	{
		OnlineSessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionCompleteDelegateHandle);
	}

	if (!OnlineSessionInterface.IsValid() || !bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameSession Interface is invailed or find session was not successful"));
		return;
	}

	

	if (!SessionSearch.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("SessionSearch is null"));
		return;
	}

	if (SessionSearch->SearchResults.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("FindSessionComplete: No Sessions found"));
		return;
	}

	bool bJoinRequested = false;

	for (auto Result : SessionSearch->SearchResults)
	{
		FString id = Result.GetSessionIdStr();
		FString user = Result.Session.OwningUserName;
		const int32 PingMs = Result.PingInMs;

		FString MatchType;
		const bool bHasMatchType = Result.Session.SessionSettings.Get(KEY_MatchType, MatchType);
		
		UE_LOG(LogTemp, Warning, TEXT("Found Session: ID=%s Owner=%s ping=%d MathType=%s (Has=%s)"),
			*id, *user, PingMs, *MatchType, bHasMatchType ? TEXT("true") : TEXT("False"));

		UE_LOG(LogTemp, Warning, TEXT("Session ID : %s / Owner : %s"), *id, *MatchType);
		
		if (bHasMatchType && MatchType == FString("FreeForAll"))
		{
			UE_LOG(LogTemp, Warning, TEXT("JOIN Match Type : %s"), *MatchType);

			const ULocalPlayer* LocalPlayer = GetWorld() ? GetWorld()->GetFirstLocalPlayerFromController() : nullptr;

			if (LocalPlayer == nullptr || !LocalPlayer->GetPreferredUniqueNetId().IsValid())
			{
				UE_LOG(LogTemp, Warning, TEXT("LocalPlayer is nullptr"));
				return;
			}


			OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &UMySessionSubsystem::JoinSessionComplete);
			JoinSessionCompleteDelegateHandle =
				OnlineSessionInterface->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

			
			const bool bJoinStarted = 
				OnlineSessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, Result);

			if (!bJoinStarted)
			{
				OnlineSessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
			}

			bJoinRequested = true;
			break;
			
		}

	}
}

void UMySessionSubsystem::JoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!OnlineSessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("OnlineSessionInterface is nullptr"));
		return;
	}

	OnlineSessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
	UE_LOG(LogTemp, Warning, TEXT("Join Session Complete: Session = %s Request = %d"),
		*SessionName.ToString(), (int32)Result);

	if (Result != EOnJoinSessionCompleteResult::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Join failed. Result = %d"), (int32)Result);
		return;
	}



	FString Adress;

	if (OnlineSessionInterface->GetResolvedConnectString(SessionName, Adress))
	{


		/*if (Adress.EndsWith(TEXT(":0")))
		{
			Adress = Adress.Replace(TEXT(":0"), TEXT(":7777"));
			UE_LOG(LogTemp, Warning, TEXT("connect String fixed to : %s"), *Adress)
		}*/

		UE_LOG(LogTemp, Warning, TEXT("connect String : %s"), *Adress);

	

		APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
		if (PlayerController)
		{
			PlayerController->ClientTravel(Adress, ETravelType::TRAVEL_Absolute);
		}

		else
		{
			UE_LOG(LogTemp, Warning, TEXT("JoinSessionComplete: PlayerController is null"));
			
		}
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GetResolvedConnectString failed"));
	}
}


void UMySessionSubsystem::OnSessionUserInviteAccept()
{
	if (!ESureOssInterfaces())
	{
		return;
	}

	if (!OnlineSessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("OnlineSession is nullptr"));
		return;
	}

	if (SessionUserInviteAcceptedDelegateHandle.IsValid())
	{
		OnlineSessionInterface->ClearOnSessionUserInviteAcceptedDelegate_Handle(SessionUserInviteAcceptedDelegateHandle);
		SessionUserInviteAcceptedDelegateHandle.Reset();
	}

	OnSessionUserInviteAcceptedDelegate = FOnSessionUserInviteAcceptedDelegate::CreateUObject(this, &UMySessionSubsystem::OnSessionUserInviteAccepted);
	SessionUserInviteAcceptedDelegateHandle = OnlineSessionInterface->AddOnSessionUserInviteAcceptedDelegate_Handle(OnSessionUserInviteAcceptedDelegate);

	


}


void UMySessionSubsystem::OnSessionUserInviteAccepted(const bool bWasSuccess, const int32 ControllerId, FUniqueNetIdPtr UserId, const FOnlineSessionSearchResult& InviteResult)
{
	UE_LOG(LogTemp, Warning, TEXT("[UMySessionSubsystem::OnSessionUserInviteAccepted] bWasSuccess = %s, ControllerId = %d"),
		bWasSuccess ? TEXT("true") : TEXT("False"), ControllerId);

	if (!bWasSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UMySessionSubsystem::OnSessionUserInviteAccepted] bWasSuccess = False"));
		return;
	}

	if (!InviteResult.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("[UMySessionSubsystem::OnSessionUserInviteAccepted] InviteReusult is nullptr"));
		return;
	}

	if (!UserId.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("[UMySessionSubsystem::OnSessionUserInviteAccepted] UserId is nullptr"));
		return;
	}

	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &UMySessionSubsystem::JoinSessionComplete);
	JoinSessionCompleteDelegateHandle =
		OnlineSessionInterface->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

	bool bjoinStarted = OnlineSessionInterface->JoinSession(*UserId, NAME_GameSession, InviteResult);

	if (!bjoinStarted)
	{
		OnlineSessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
		JoinSessionCompleteDelegateHandle.Reset();
	}

}

void UMySessionSubsystem::ShowInviteUI()
{

	onlinesubsystem = IOnlineSubsystem::Get();

	if (onlinesubsystem)
	{
		IOnlineExternalUIPtr onlineExternalUI = onlinesubsystem->GetExternalUIInterface();

		bool bshowinvite = onlineExternalUI->ShowInviteUI(0, NAME_GameSession);

		UE_LOG(LogTemp, Warning, TEXT("[UMySessionSubsystem::ShowInviteUI] showInviteUI = %s"), bshowinvite ? TEXT("true") : TEXT("false"));
	}

	
	
}

void UMySessionSubsystem::ReturnToLobby()
{
	if (!ESureOssInterfaces())
	{
		return;
	}

	const auto ExisitingSession = OnlineSessionInterface->GetNamedSession(NAME_GameSession);

	if (ExisitingSession != nullptr)
	{
		OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this,&UMySessionSubsystem::ReturnToLobbyComplete);
		DestroySessionCompleteDelegateHandle = OnlineSessionInterface->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);


		bool bDestroyed = OnlineSessionInterface->DestroySession(NAME_GameSession);

		if (!bDestroyed)
		{
			OnlineSessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
		}

		UE_LOG(LogTemp, Warning, TEXT("ExisitingSession Found. Destroy Requested. TryAgain"));
		return;
	}

}

void UMySessionSubsystem::ReturnToLobbyComplete(FName SessionName, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UMySessionSubsystem::ReturnToLobbyComplete] !bWasSuccessful"));
		return;
	}


	if (!OnlineSessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("[[UMySessionSubsystem::ReturnToLobbyComplete]] !OnlineSessionInterface.IsValid"));
		return;
	}
	
	OnSessionDestroyComplete.Broadcast(bWasSuccessful);

	OnlineSessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
}


