#include "DemonKing/Player/RoguePlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "Blueprint/UserWidget.h"
#include "DemonKing/RogueHUD/RogueHUD.h"
#include "DemonKing/GameFlow/RogueGameModeBase.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Engine/LocalPlayer.h"
#include "DemonKing/GameFlow/RoguePlayerState.h"
#include "DemonKing/Online/MySessionSubsystem.h"
#include "DemonKing/CCharacter/RogueCharacterBase.h"


ARoguePlayerController::ARoguePlayerController()
{
	
}

void ARoguePlayerController::BeginPlay()
{
	Super::BeginPlay();
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	UpdateWorldName();
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ARoguePlayerController::AfterChangeWorldMap);


	UMySessionSubsystem* mySubsystem = GetGameInstance()? GetGameInstance()->GetSubsystem<UMySessionSubsystem>() : nullptr;

	if (mySubsystem)
	{
		mySubsystem->OnSessionDestroyComplete.AddDynamic(this, &ARoguePlayerController::BackToLobby);
	}

	characterBase = Cast<ARogueCharacterBase>(GetPawn());

}


void ARoguePlayerController::Tick(float Deltatime)
{
	Super::Tick(Deltatime);
	LookMouseCursor();
}


void ARoguePlayerController::LookMouseCursor()
{
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		APawn* const MyPawn = GetPawn();

		if (MyPawn)
		{
			FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(
				MyPawn->GetActorLocation(), FVector(Hit.Location.X, Hit.Location.Y, MyPawn->GetActorLocation().Z));

			FRotator NewRotation = FMath::RInterpTo(
				MyPawn->GetActorRotation(),
				TargetRotation, GetWorld()->GetDeltaSeconds(), 20.0f);

			MyPawn->SetActorRotation(NewRotation);
		}

		
	}
}

void ARoguePlayerController::AfterChangeWorldMap(UWorld* LoadedWorld)
{
	if (!IsLocalController())
	{
		return;
	}

	UpdateWorldName();
}


void ARoguePlayerController::SetupInputComponent()
{

	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveForward", this, &ARoguePlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ARoguePlayerController::MoveRight);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ARoguePlayerController::OnJumpPressed);
	InputComponent->BindAction("Jump", IE_Released, this, &ARoguePlayerController::OnJumpReleased);

	InputComponent->BindAction("Autoattack", IE_Pressed, this, &ARoguePlayerController::OnMouseLeftClick);
	InputComponent->BindAction("SkillE", IE_Pressed, this, &ARoguePlayerController::OnEPressed);
	InputComponent->BindAction("SkillQ", IE_Pressed, this, &ARoguePlayerController::OnQPressed);


}



void ARoguePlayerController::MoveForward(float value)
{
	APawn* p = GetPawn();

	if (!p)
	{
		return;
	}
	
	p->AddMovementInput(p->GetActorForwardVector(), value);
}

void ARoguePlayerController::MoveRight(float value)
{
	APawn* p = GetPawn();

	if (!p)
	{
		return;
	}
	p->AddMovementInput(FVector::RightVector, value);
}

void ARoguePlayerController::OnJumpPressed()
{
	ACharacter* C = Cast<ACharacter>(GetPawn());
	if (!C)
	{
		return;
	}

	C->Jump();
}

void ARoguePlayerController::OnJumpReleased()
{
	ACharacter* C = Cast<ACharacter>(GetPawn());

	if (!C) return;

	C->StopJumping();
}

void ARoguePlayerController::OnMouseLeftClick()
{
	if (!characterBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("characterBase failed casting"));
		return;
	}

	
	characterBase->InputSkillLeftMouse();
}

void ARoguePlayerController::OnQPressed()
{
	if (!characterBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("characterBase failed casting"));
		return;
	}
	characterBase->InputSkillQ();
}

void ARoguePlayerController::OnEPressed()
{
	if (!characterBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("characterBase failed casting"));
		return;
	}
	characterBase->InputSkillE();
}

void ARoguePlayerController::ApplyMode(ETypeControll controll)
{

	UE_LOG(LogTemp, Warning, TEXT("[ARoguePlayerController::ApplyMode]"));
	 Mode = controll;

	 if (!IsLocalController()) return;

	
	switch (Mode) {
	case ETypeControll::Main:
	{
		FInputModeUIOnly inputmode;
		SetInputMode(inputmode);
		if (ARogueHUD* hud = GetHUD<ARogueHUD>())
		{
			hud->ShowMainMenuWidget();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("failed get hud"));
		}
		break;
	}
	case ETypeControll::Lobby:
	{
		FInputModeUIOnly inputmode;
		SetInputMode(inputmode);
		if (ARogueHUD* hud = GetHUD<ARogueHUD>())
		{
			hud->ShowStartGameWidget();
		}
		SubmitMyLobbyNickName();
		break;
	}

	case ETypeControll::Game:
	{
		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);
		break;
		
	}
	default:
		break;
	}
	
}

void ARoguePlayerController::UpdateWorldName()
{
	FString mapname = GetWorld()->GetMapName();
	mapname.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

	if (mapname == TEXT("L_MainMenu"))
	{
		ApplyMode(ETypeControll::Main);

	}

	else if (mapname == TEXT("L_Lobby"))
	{
		ApplyMode(ETypeControll::Lobby);
	}

	else
	{
		ApplyMode(ETypeControll::Game);
	}

}

FString ARoguePlayerController::GetPlayerNickName()
{
	ULocalPlayer* player = GetLocalPlayer();

	if (player)
	{
		FUniqueNetIdRepl nickname = player->GetPreferredUniqueNetId();

		if (nickname.IsValid())
		{
			
			IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();

			if (!OnlineSubsystem)
			{
				return FString();
			}

			IOnlineIdentityPtr identity = OnlineSubsystem->GetIdentityInterface();

			if (!identity)
			{
				return FString();
			}

			FUniqueNetIdPtr uniqueNetid = nickname.GetUniqueNetId();

			if (!uniqueNetid)
			{
				return FString();
			}

			return identity->GetPlayerNickname(*uniqueNetid);

				
		}
	}

	return FString();
}

void ARoguePlayerController::SubmitMyLobbyNickName()
{
	UE_LOG(LogTemp, Warning, TEXT("[ARoguePlayerController::SubmitMyLobbyNickName]"));

	FString NewNicKName = GetPlayerNickName();

	if (!NewNicKName.IsEmpty())
	{
		Server_SetLobbyNickName(NewNicKName);
	}
}

void ARoguePlayerController::BackToLobby(bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("bWasSuccessful = %s"), bWasSuccessful ? TEXT("true") : TEXT("False"));
	this->ClientTravel("/Game/Maps/L_MainMenu", TRAVEL_Absolute);
}

void ARoguePlayerController::Server_SetLobbyNickName_Implementation(const FString& newname)
{
	if (!HasAuthority())
	{
		return;
	}

	ARoguePlayerState* RoguePS = GetPlayerState<ARoguePlayerState>();

	if (!RoguePS)
	{
		return;
	}

	FString NewNickName = newname;

	RoguePS->SetLobbyPlayerNickName(NewNickName);
}

void ARoguePlayerController::Server_RequestStartRun_Implementation()
{
	if (!HasAuthority())
	{
		return;
	}


	if (ARogueGameModeBase* GM = GetWorld()->GetAuthGameMode<ARogueGameModeBase>())
	{
		GM->StartRun();
	}
}