#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RoguePlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyNickNameChanged, const FString&, NewNickName);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerHpChanged, float);

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

	void SetPlayerState_HP(float hp);
	FORCEINLINE float GetPlayerState_HP() { return CurrentHpPercent;}


protected:
	UFUNCTION()
	void OnRep_PlayerNickName();

	UFUNCTION()
	void OnRep_PlayerHP();


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(ReplicatedUsing = OnRep_PlayerHP, BlueprintReadOnly, Category = "State")
	float CurrentHpPercent = 0.0f;


	                       
public:
	UPROPERTY(ReplicatedUsing = OnRep_PlayerNickName, BlueprintReadOnly, Category = "NickName")
	FString PlayerNickName;

	UPROPERTY(BlueprintAssignable, Category = "NickName")
	FOnLobbyNickNameChanged OnLobbyNickNameChanged;

	FOnPlayerHpChanged OnPlayerHpChanged;





	
};
