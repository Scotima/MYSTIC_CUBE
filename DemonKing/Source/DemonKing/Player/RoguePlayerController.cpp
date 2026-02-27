#include "DemonKing/Player/RoguePlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"

void ARoguePlayerController::BeginPlay()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
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
