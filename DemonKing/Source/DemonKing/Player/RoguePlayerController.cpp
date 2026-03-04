#include "DemonKing/Player/RoguePlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "Blueprint/UserWidget.h"
#include "DemonKing/RogueHUD/RogueHUD.h"
#include "DemonKing/GameFlow/RogueGameModeBase.h"

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

void ARoguePlayerController::ApplyMode(ETypeControll controll)
{
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

	else if (mapname == TEXT("TESTMAP"))
	{
		ApplyMode(ETypeControll::Lobby);
	}

	else
	{
		ApplyMode(ETypeControll::Game);
	}

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