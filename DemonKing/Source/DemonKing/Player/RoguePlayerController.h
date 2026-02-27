#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RoguePlayerController.generated.h"


UCLASS()
class DEMONKING_API ARoguePlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay()override;
	virtual void Tick(float DeltaTime) override;

public:
	//https://parkcy723.tistory.com/104 ÄÚµå

	UFUNCTION()
	void LookMouseCursor();

private:
	virtual void SetupInputComponent() override;


private:
	void MoveForward(float value);
	void MoveRight(float value);

	void OnJumpPressed();
	void OnJumpReleased();


	
	
};
