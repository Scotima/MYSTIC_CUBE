#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RoguePlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyNickNameChanged, const FString&, NewNickName);

UCLASS()
class DEMONKING_API ARoguePlayerState : public APlayerState
{
	GENERATED_BODY()


protected:
	ARoguePlayerState();

public:
	UFUNCTION(BlueprintCallable, Category = "NickName")
	void SetLobbyPlayerNickName(FString& NickName);

	UFUNCTION(BlueprintCallable, Category = "NickName")
	FString GetLobbyPlayerNickName();


protected:
	UFUNCTION()
	void OnRep_PlayerNickName();


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	                       
public:
	UPROPERTY(ReplicatedUsing = OnRep_PlayerNickName, BlueprintReadOnly, Category = "NickName")
	FString PlayerNickName;

	UPROPERTY(BlueprintAssignable, Category = "NickName")
	FOnLobbyNickNameChanged OnLobbyNickNameChanged;


	
};
