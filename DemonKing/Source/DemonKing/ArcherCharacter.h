#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacterBase.h"
#include "ArcherCharacter.generated.h"

class AArrowProjectile;
class UNiagaraSystem;

UCLASS()
class DEMONKING_API AArcherCharacter : public APlayerCharacterBase
{
	GENERATED_BODY()

public:
	AArcherCharacter();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Archer|Attack")
	TSubclassOf<AArrowProjectile> ArrowProjectileClass;

    UFUNCTION(Server, Reliable)
    void ServerCastArrowRain(FVector TargetLocation);

    UFUNCTION(Client, Reliable)
    void ClientArrowRainCastRejected();

    UFUNCTION(NetMulticast, Reliable, BlueprintCallable,
        Category = "Archer|ArrowRain")
    void MulticastSpawnArrowRainEffect(
        UNiagaraSystem* Effect,
        FVector TargetLocation);

public:
    virtual void Tick(float DeltaTime) override;
    virtual void InputSkillE() override;
    void BeginArrowRainTargeting();
    void CancelArrowRainTargeting();

    bool IsArrowRainTargeting() const
    {
        return bIsAimingArrowRain;
    }
    virtual void SetupPlayerInputComponent(
        class UInputComponent* PlayerInputComponent) override;

    void ConfirmArrowRainTargeting();

protected:
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly,
        Category = "Archer|ArrowRain")
    bool bIsAimingArrowRain = false;

    void UpdateArrowRainTargeting();

    // 우선 표시 확인용 임시 반지름
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
        Category = "Archer|ArrowRain", meta = (ClampMin = "1.0"))
    float ArrowRainRadius = 300.0f;

    // 카메라로 구한 목표 거리를 얼마나 늘릴지 설정
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
        Category = "Archer|ArrowRain", meta = (ClampMin = "0.1"))
    float ArrowRainDistanceScale = 1.5f;

    FVector ArrowRainTargetLocation = FVector::ZeroVector;
    bool bHasValidArrowRainTarget = false;

    FVector ConfirmedArrowRainLocation = FVector::ZeroVector;
};
