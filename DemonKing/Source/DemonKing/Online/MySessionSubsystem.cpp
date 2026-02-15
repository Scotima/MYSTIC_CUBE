#include "DemonKing/Online/MySessionSubsystem.h"
#include "OnlineSubsystem.h"
#include "Templates/SharedPointer.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"

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
		UE_LOG(LogTemp, Warning, TEXT("ExisitingSession Found. Destory Requested. TryAgain"));
		return;
	}
	

	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UMySessionSubsystem::MakeSessionComplete);


	CreateSessionCompleteDelegateHandle = OnlineSessionInterface->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

	//세션 세팅
	TSharedPtr<FOnlineSessionSettings> SessionSettings = MakeShareable(new FOnlineSessionSettings());

	SessionSettings->bIsLANMatch = true; //null 테스트를 위해서.
	SessionSettings->NumPublicConnections = 2;
	SessionSettings->bAllowJoinInProgress = false; // 세션 실행중 참여 여부.
	SessionSettings->bAllowJoinViaPresence = true; 
	SessionSettings->bShouldAdvertise = true; // 스팀을 통해 세션 알림.
	SessionSettings->bUsesPresence = false;
	SessionSettings->bUseLobbiesIfAvailable = false; //로비 사용 여부.
	SessionSettings->Set(FName("MatchType"), FString("FreeForAll"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

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
		//서버 연결하기.
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Session failed"));
	}
}

//https://zeniff.tistory.com/23 코드.

void UMySessionSubsystem::FindSession()
{
	if (!OnlineSessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("GameSession Interface is invailed"));
		return;
	}

	OnlineSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionCompleteDelegate);

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->MaxSearchResults = 100;
	SessionSearch->bIsLanQuery = true;
	//SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals); <- 스팀 확장 하면 고치기.

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	OnlineSessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef());

}


