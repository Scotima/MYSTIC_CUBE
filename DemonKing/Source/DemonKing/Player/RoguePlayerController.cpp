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
#include "RoguePlayerController.h"
#include "DemonKing/SkillComponent/CKnightSkillComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerState.h"


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
	//LookMouseCursor();
}


void ARoguePlayerController::LookMouseCursor()
{
	APawn* const MyPawn = GetPawn();

	

	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{

		if (MyPawn)
		{
			FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(
				MyPawn->GetActorLocation(), FVector(Hit.Location.X, Hit.Location.Y, MyPawn->GetActorLocation().Z));

			FRotator NewRotation = FMath::RInterpTo(
				MyPawn->GetActorRotation(),
				TargetRotation, GetWorld()->GetDeltaSeconds(), 20.0f);

			MyPawn->SetActorRotation(TargetRotation);
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


void ARoguePlayerController::Server_SelectPlayerClass_Implementation(EPlayerClassType PlayerClass)
{
	//BP에서 호출하기.
	APlayerState* PS = GetPlayerState<APlayerState>();

	UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());

	if (!IsValid(PS) || !IsValid(GI))
	{
		UE_LOG(LogTemp, Warning,
			TEXT("[ClassSelect] Failed: PS=%s, GI=%s"),
			IsValid(PS) ? TEXT("Valid") : TEXT("Invalid"),
			IsValid(GI) ? TEXT("Valid") : TEXT("Invalid"));
		return;
	}

	const FUniqueNetIdRepl& UniqueId = PS->GetUniqueId();

	if (!UniqueId.IsValid())
	{
		return;
	}

	const FString PlayerKey = UniqueId.ToString();


	GI->SetPlayerClassById(PlayerKey, PlayerClass);
	UE_LOG(LogTemp, Warning,
		TEXT("[ClassSelect] Save called: GI=%p, PlayerId=%d, Class=%d"),
		static_cast<const void*>(GI),
		PS->GetPlayerId(),
		static_cast<int32>(PlayerClass));
}

void ARoguePlayerController::SetupInputComponent()
{

	Super::SetupInputComponent();
	InputComponent->BindAxis("Turn", this, &ARoguePlayerController::Turn);
	InputComponent->BindAxis("LookUp", this, &ARoguePlayerController::LookUp);
	InputComponent->BindAxis("MoveForward", this, &ARoguePlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ARoguePlayerController::MoveRight);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ARoguePlayerController::OnJumpPressed);
	InputComponent->BindAction("Jump", IE_Released, this, &ARoguePlayerController::OnJumpReleased);

	InputComponent->BindAction("Autoattack", IE_Pressed, this, &ARoguePlayerController::OnMouseLeftClick);
	InputComponent->BindAction("Autoattack", IE_Released, this, &ARoguePlayerController::OnMouseLeftReleased);
	InputComponent->BindAction("SkillE", IE_Pressed, this, &ARoguePlayerController::OnEPressed);
	InputComponent->BindAction("SkillE", IE_Released, this, &ARoguePlayerController::OnEDePressed);
	InputComponent->BindAction("SkillQ", IE_Pressed, this, &ARoguePlayerController::OnQPressed);
	InputComponent->BindAction("SkillQ", IE_Released, this, &ARoguePlayerController::OnQDePressed);

	InputComponent->BindAction("SkillShift", IE_Pressed, this, & ARoguePlayerController::OnShiftPressed);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (PauseMenuAction)
		{
			EnhancedInput->BindAction(PauseMenuAction, ETriggerEvent::Started, this, &ARoguePlayerController::OnPauseMenuPressed);
		}
	}

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

		if (Subsystem && PauseMenuMappingContext)
		{
			Subsystem->AddMappingContext(PauseMenuMappingContext, 0);
		}
	}


}



void ARoguePlayerController::MoveForward(float value)
{
	if (!CanUseGameplayInput())
	{
		return;
	}

	APawn* p = GetPawn();
	FRotator ControlRot = FRotator(0, GetControlRotation().Yaw, 0);
	FVector Direction = FQuat(ControlRot).GetForwardVector().GetSafeNormal2D();

	if (!p)
	{
		return;
	}
	
	p->AddMovementInput(Direction, value);
}

void ARoguePlayerController::MoveRight(float value)
{
	if (!CanUseGameplayInput())
	{
		return;
	}

	APawn* p = GetPawn();

	FRotator ControlRot = FRotator(0, GetControlRotation().Yaw, 0);
	FVector Direction = FQuat(ControlRot).GetRightVector().GetSafeNormal2D();

	if (!p)
	{
		return;
	}
	p->AddMovementInput(Direction, value);
}

void ARoguePlayerController::OnJumpPressed()
{
	if (!CanUseGameplayInput())
	{
		return;
	}

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
	if (!CanUseGameplayInput())
	{
		return;
	}

	characterBase = Cast<ARogueCharacterBase>(GetPawn());
	UE_LOG(LogTemp, Warning, TEXT("[ARoguePlayerController::OnMouseLeftClick]"));
	if (!characterBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("characterBase failed casting"));
		return;
	}
    characterBase->SetUsingSkill(true);
	characterBase->ServerSkillInput(0, true); // 마우스 왼쪽 클릭 서버 호출.

}

void ARoguePlayerController::OnMouseLeftReleased()
{
	if(!characterBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterBase failed casting"));
		return;
	}

	characterBase->SetUsingSkill(false);
	characterBase->ServerSkillInput(0, false);
}
void ARoguePlayerController::OnQPressed()
{
	if (!CanUseGameplayInput())
	{
		return;
	}

	characterBase = Cast<ARogueCharacterBase>(GetPawn());

	if (!characterBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("characterBase failed casting"));
		return;
	}
	characterBase->SetUsingSkill(true);
	characterBase->ServerSkillInput(1); // Q 스킬 서버 호출.
}

void ARoguePlayerController::OnQDePressed()
{
	if (!characterBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("characterBase failed casting"));
		return;
	}
	characterBase->SetUsingSkill(false);
}

void ARoguePlayerController::OnEPressed()
{
	if (!CanUseGameplayInput())
	{
		return;
	}

	characterBase = Cast<ARogueCharacterBase>(GetPawn());
	if (!characterBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("characterBase failed casting"));
		return;
	}
	characterBase->SetUsingSkill(true);
	characterBase->ServerSkillInput(2);
}

void ARoguePlayerController::OnEDePressed()
{
	if (!characterBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("characterBase failed casting"));
		return;
	}
	characterBase->SetUsingSkill(false);
}

void ARoguePlayerController::OnShiftPressed()
{
	if (!CanUseGameplayInput())
	{
		return;
	}

	characterBase = Cast<ARogueCharacterBase>(GetPawn());
	if(!characterBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("characterBase failed casting"));
		return;
	}
	characterBase->ServerSkillInput(3);

	//대쉬 중인지 판단하는 변수 만들어서 wasd입력값 못받게 하기.
	//끝나면 노티파이로 호출해서 다시 true로 만들기. 간단하게.
}

void ARoguePlayerController::OnPauseMenuPressed()
{
	if (!IsLocalController() || Mode != ETypeControll::Game)
	{
		return;
	}

	ARogueHUD* RogueHUD = Cast<ARogueHUD>(GetHUD());
	if (!IsValid(RogueHUD))
	{
		return;
	}

	RogueHUD->ShowPauseMenuWidget();
	UUserWidget* PauseWidget = RogueHUD->GetPauseMenuWidget();
	RogueHUD->SetOnOFF(IsValid(PauseWidget) && PauseWidget->IsInViewport());
	RefreshCursorInputMode();
}

void ARoguePlayerController::ApplyMode(ETypeControll controll)
{

	UE_LOG(LogTemp, Warning, TEXT("[ARoguePlayerController::ApplyMode]"));
	 Mode = controll;

	 if (!IsLocalController()) return;

	
	switch (Mode) {
	case ETypeControll::Main:
	{
		
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

		if (ARogueHUD* hud = GetHUD<ARogueHUD>())
		{
			hud->ShowSkillBarHUD();
		}

		break;
		
	}
	default:
		break;
	}
	RefreshCursorInputMode();
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

bool ARoguePlayerController::CanUseGameplayInput() const
{
	ARogueHUD* RogueHUD = Cast<ARogueHUD>(GetHUD());
	const bool bPauseMenuOpen = RogueHUD && RogueHUD->GetOnOFF();

	return IsLocalController()
		&& Mode == ETypeControll::Game
		&& !bItemSelectionOpen
		&& !bPauseMenuOpen;
}

void ARoguePlayerController::SetItemSelectionOpen(bool bOpen, UUserWidget* Widget)
{
	if (!IsLocalController() || (bOpen && !IsValid(Widget)))
	{
		return;
	}

	if (!bOpen && ItemSelectionWidget.Get() != Widget)
	{
		return;
	}

	bItemSelectionOpen = bOpen;
	ItemSelectionWidget = bOpen ? Widget : nullptr;
	RefreshCursorInputMode();
}

void ARoguePlayerController::RefreshCursorInputMode()
{
	if (!IsLocalController())
	{
		return;
	}

	ARogueHUD* RogueHUD = Cast<ARogueHUD>(GetHUD());
	const bool bPauseMenuOpen = RogueHUD && RogueHUD->GetOnOFF();
	const bool bShowUI = Mode != ETypeControll::Game
		|| bItemSelectionOpen || bPauseMenuOpen;

	bShowMouseCursor = bShowUI;
	bEnableClickEvents = bShowUI;
	bEnableMouseOverEvents = bShowUI;

	if (bShowUI)
	{
		if (ACharacter* PawnCharacter = Cast<ACharacter>(GetPawn()))
		{
			PawnCharacter->StopJumping();
		}
		if (ARogueCharacterBase* RogueCharacter = Cast<ARogueCharacterBase>(GetPawn()))
		{
			RogueCharacter->SetUsingSkill(false);
			RogueCharacter->ServerSkillInput(0, false);
		}
	}

	if (Mode != ETypeControll::Game)
	{
		SetInputMode(FInputModeUIOnly());
	}
	else if (bShowUI)
	{
		UUserWidget* FocusWidget = bPauseMenuOpen
			? RogueHUD->GetPauseMenuWidget() : ItemSelectionWidget.Get();

		FInputModeGameAndUI InputMode;
		InputMode.SetHideCursorDuringCapture(false);
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		if (IsValid(FocusWidget))
		{
			InputMode.SetWidgetToFocus(FocusWidget->TakeWidget());
		}
		SetInputMode(InputMode);
	}
	else
	{
		FInputModeGameOnly InputMode;
		InputMode.SetConsumeCaptureMouseDown(true);
		SetInputMode(InputMode);
	}
}

void ARoguePlayerController::Turn(float Value)
{
	if (CanUseGameplayInput())
	{
		AddYawInput(Value);
	}
}

void ARoguePlayerController::LookUp(float Value)
{
	if (CanUseGameplayInput())
	{
		AddPitchInput(Value);
	}
}