#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacterBase.h"
#include "ArcherCharacter.generated.h"

class AArrowProjectile;

UCLASS()
class DEMONKING_API AArcherCharacter : public APlayerCharacterBase
{
	GENERATED_BODY()

public:
	AArcherCharacter();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Archer|Attack")
	TSubclassOf<AArrowProjectile> ArrowProjectileClass;
};
