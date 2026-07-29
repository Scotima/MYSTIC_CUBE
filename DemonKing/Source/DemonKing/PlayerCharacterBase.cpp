#include "PlayerCharacterBase.h"

#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Controller.h"

APlayerCharacterBase::APlayerCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APlayerCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	// 프로젝트 Input 설정에 아래 이름이 있어야 함
	PlayerInputComponent->BindAction("SkillQ", IE_Pressed, this, &APlayerCharacterBase::InputSkillQ);
	PlayerInputComponent->BindAction("SkillE", IE_Pressed, this, &APlayerCharacterBase::InputSkillE);
	PlayerInputComponent->BindAction("SkillShift", IE_Pressed, this, &APlayerCharacterBase::InputSkillShift);
}

bool APlayerCharacterBase::CanUseSkill(ESkillSlot SkillSlot) const
{
	if (bIsCastingSkill)
	{
		return false;
	}

	const UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}

	const float TimeNow = World->GetTimeSeconds();

	switch (SkillSlot)
	{
	case ESkillSlot::Q:
		return CurrentMana >= SkillQManaCost && (TimeNow - LastSkillQTime) >= SkillQCooldown;

	case ESkillSlot::E:
		return CurrentMana >= SkillEManaCost && (TimeNow - LastSkillETime) >= SkillECooldown;

	case ESkillSlot::Shift:
		return CurrentMana >= SkillShiftManaCost && (TimeNow - LastSkillShiftTime) >= SkillShiftCooldown;
	}

	return false;
}

void APlayerCharacterBase::CommitSkill(ESkillSlot SkillSlot)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const float TimeNow = World->GetTimeSeconds();

	switch (SkillSlot)
	{
	case ESkillSlot::Q:
		CurrentMana -= SkillQManaCost;
		LastSkillQTime = TimeNow;
		break;

	case ESkillSlot::E:
		CurrentMana -= SkillEManaCost;
		LastSkillETime = TimeNow;
		break;

	case ESkillSlot::Shift:
		CurrentMana -= SkillShiftManaCost;
		LastSkillShiftTime = TimeNow;
		break;
	}
}

float APlayerCharacterBase::GetCooldownRemaining(ESkillSlot SkillSlot) const
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		return 0.0f;
	}

	const float TimeNow = World->GetTimeSeconds();

	switch (SkillSlot)
	{
	case ESkillSlot::Q:
		return FMath::Max(0.0f, SkillQCooldown - (TimeNow - LastSkillQTime));

	case ESkillSlot::E:
		return FMath::Max(0.0f, SkillECooldown - (TimeNow - LastSkillETime));

	case ESkillSlot::Shift:
		return FMath::Max(0.0f, SkillShiftCooldown - (TimeNow - LastSkillShiftTime));
	}

	return 0.0f;
}

void APlayerCharacterBase::InputSkillQ()
{
	if (!CanUseSkill(ESkillSlot::Q))
	{
		return;
	}

	bIsCastingSkill = true;

	CommitSkill(ESkillSlot::Q);

	BP_UseSkillQ();

	bIsCastingSkill = false; // 추가
}

void APlayerCharacterBase::InputSkillE()
{
	if (!CanUseSkill(ESkillSlot::E))
	{
		return;
	}

	bIsCastingSkill = true;

	CommitSkill(ESkillSlot::E);

	BP_UseSkillE();

	bIsCastingSkill = false; // 추가
}

void APlayerCharacterBase::InputSkillShift()
{
	if (!CanUseSkill(ESkillSlot::Shift))
	{
		return;
	}

	bIsCastingSkill = true;

	CommitSkill(ESkillSlot::Shift);

	BP_UseSkillShift();

	bIsCastingSkill = false; // 추가
}

void APlayerCharacterBase::PerformSkillQHitCheck()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const FVector Start = GetActorLocation() + GetActorForwardVector() * 80.0f;
	const FVector End = Start + GetActorForwardVector() * SkillQRange;

	FCollisionShape Sphere = FCollisionShape::MakeSphere(SkillQRadius);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	TArray<FHitResult> HitResults;
	const bool bHit = World->SweepMultiByChannel(
		HitResults,
		Start,
		End,
		FQuat::Identity,
		ECC_Pawn,
		Sphere,
		Params
	);

	// 디버그 확인용
	/*DrawDebugCapsule(
		World,
		(Start + End) * 0.5f,
		SkillQRange * 0.5f,
		SkillQRadius,
		FRotationMatrix::MakeFromZ((End - Start).GetSafeNormal()).ToQuat(),
		bHit ? FColor::Red : FColor::Green,
		false,
		1.0f
	);
	*/

	if (!bHit)
	{
		return;
	}

	for (const FHitResult& Hit : HitResults)
	{
		AActor* HitActor = Hit.GetActor();
		if (!HitActor)
		{
			continue;
		}

		UE_LOG(LogTemp, Warning, TEXT("Skill Q Hit: %s"), *HitActor->GetName());

		// 나중에 여기서 데미지 적용
		// UGameplayStatics::ApplyDamage(...) 같은 걸 붙이면 됨
	}
}
void APlayerCharacterBase::SetCastingSkill(bool bNewCasting)
{
	bIsCastingSkill = bNewCasting;
}
void APlayerCharacterBase::PerformSkillEHitCheck()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const FVector Start = GetActorLocation() + GetActorForwardVector() * 40.0f;
	const FVector End = Start + GetActorForwardVector() * SkillERange;

	FCollisionShape Sphere = FCollisionShape::MakeSphere(SkillERadius);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	TArray<FHitResult> HitResults;
	const bool bHit = World->SweepMultiByChannel(
		HitResults,
		Start,
		End,
		FQuat::Identity,
		ECC_Pawn,
		Sphere,
		Params
	);

	if (!bHit)
	{
		return;
	}

	for (const FHitResult& Hit : HitResults)
	{
		AActor* HitActor = Hit.GetActor();
		if (!HitActor)
		{
			continue;
		}

		UE_LOG(LogTemp, Warning, TEXT("Skill E Hit: %s"), *HitActor->GetName());

		// 나중에 ApplyDamage 추가
	}
}

void APlayerCharacterBase::ActivateShieldSkill()
{
	bShieldActive = true;
	CurrentShieldAmount = MaxShieldAmount;

	if (GetWorld())
	{
		GetWorldTimerManager().ClearTimer(ShieldTimerHandle);
		GetWorldTimerManager().SetTimer(
			ShieldTimerHandle,
			this,
			&APlayerCharacterBase::EndShieldSkill,
			ShieldDuration,
			false
		);
	}

	UE_LOG(LogTemp, Warning, TEXT("Shield Activated"));
}

void APlayerCharacterBase::EndShieldSkill()
{
	bShieldActive = false;
	CurrentShieldAmount = 0.0f;

	UE_LOG(LogTemp, Warning, TEXT("Shield Ended"));
}