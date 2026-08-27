#include "DemonKing/ActorComponent/EnemyComponent/CEnemyStatComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/EngineTypes.h"
#include "DemonKing/ActorComponent/PlayerComponent/CCharacterStatComponent.h"
#include "DemonKing/Item/CItemBase.h"
#include "Engine/World.h"
UCEnemyStatComponent::UCEnemyStatComponent()
{

	PrimaryComponentTick.bCanEverTick = true;
}

void UCEnemyStatComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHp = MaxHp;
}



void UCEnemyStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

void UCEnemyStatComponent::AttackPlayer(AActor* HitActor)
{
	UCCharacterStatComponent* Player = HitActor->FindComponentByClass<UCCharacterStatComponent>();

	if (!Player)
	{
		return;
	}

	Player->TakeDamage(AttackPower);
}

void UCEnemyStatComponent::TakeDamage(float IncomingDamage)
{
	TakeDamage(IncomingDamage, 0.0f, 0.0f);
}

void UCEnemyStatComponent::TakeDamage(float IncomingDamage,float DefensePenetration, float DefenseIgnoreRate)
{
	if (isDead || !bCanBeDamaged || CurrentHp <= 0.0f)
	{
		return; //Aready Hp is Zero return
	}

	const float FinalDamage = CalculateFinalDamageTaken(IncomingDamage, DefensePenetration, DefenseIgnoreRate);
	CurrentHp = FMath::Clamp(CurrentHp - FinalDamage, 0.0f, MaxHp);
	
	
	if (CurrentHp <= 0.0f)
	{
		Die();
	}

}

float UCEnemyStatComponent::CalculateFinalDamageTaken(float IncomingDamage, float DefensePenetration, float DefenseIgnoreRate) const
{
	const float ClampedDefenseIgnoreRate = FMath::Clamp(DefenseIgnoreRate, 0.0f, 1.0f);
	const float EffectiveDefense = FMath::Max(0.0f, Defense - DefensePenetration) * (1.0f - ClampedDefenseIgnoreRate);
	const float DamageReduction = FMath::Clamp(EffectiveDefense / (EffectiveDefense + ArmorK), 0.0f, MaxDamageReduction);

	return FMath::Max(0.0f, IncomingDamage)
		* FMath::Max(0.0f, 1.0f + DamageTakenAmpSum)
		* (1.0f - DamageReduction);
}

void UCEnemyStatComponent::AddDamageTakenAmp(float IncreaseRate)
{
	DamageTakenAmpSum += IncreaseRate;
}

void UCEnemyStatComponent::RemoveDamageTakenAmp(float IncreaseRate)
{
	DamageTakenAmpSum -= IncreaseRate;
}

void UCEnemyStatComponent::DoTrace(const FBoxTraceData& BoxTraceData)
{
	AActor* Owner = GetOwner();

	if (!Owner)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UCKnightSkillComponent::DoTrace] !Owner"));
		return;
	}


	const float Alpha = FMath::Clamp(BoxTraceData.Value, 0.0f, 1.0f);

	const float TraceDistance = FMath::Lerp(BoxTraceData.MaxDistance, BoxTraceData.MinDistance, Alpha);
	const FVector BoxHalfSize = FMath::Lerp(BoxTraceData.MaxBoxHalfSize, BoxTraceData.MinBoxHalfSize, Alpha);

	FVector TraceDir = Owner->GetActorForwardVector();

	switch (BoxTraceData.BoxTraceDirection)
	{
	case EBoxTraceDirection::FORWARD:
		TraceDir = Owner->GetActorForwardVector();
		break;
	case EBoxTraceDirection::BACKWARD:
		TraceDir = -Owner->GetActorForwardVector();
		break;
	case EBoxTraceDirection::RIGHT:
		TraceDir = Owner->GetActorRightVector();
		break;
	case EBoxTraceDirection::LEFT:
		TraceDir = -Owner->GetActorRightVector();
		break;
	default:
		break;
	}



	const FVector Start = Owner->GetActorLocation() + TraceDir * BoxTraceData.StartDistance;
	const FVector End = Start + TraceDir * TraceDistance;

	TArray<FHitResult> HitResults;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(Owner);

	bool Hit = UKismetSystemLibrary::BoxTraceMulti(this,
		Start, End, BoxHalfSize, TraceDir.Rotation(), UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false, IgnoreActors, EDrawDebugTrace::ForDuration, HitResults, true);


	if (Hit)
	{
		TSet<AActor*> AlreadyHitActors;
		for (const FHitResult& HitResult : HitResults)
		{
			AActor* HitActor = HitResult.GetActor();

			if (!HitActor || HitActor == Owner)
			{
				continue;
			}

			if (AlreadyHitActors.Contains(HitActor))
			{
				continue;
			}

			AlreadyHitActors.Add(HitActor);

			AttackPlayer(HitActor);
			
		}
	}
}

void UCEnemyStatComponent::Die()
{
	//call function Enemy->PlayMontage and Destroy this;
	if (isDead)
	{
		return;
	}

	isDead = true;
	bCanBeDamaged = false;

	AActor* Owner = GetOwner();

	if (!IsValid(Owner))
	{
		return;
	}

	DropItem();
	Owner->Destroy();
	
}

void UCEnemyStatComponent::DropItem()
{
	if (!Item)
	{
		return;
	}
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	FVector SpawnLocation = Owner->GetActorLocation();
	FRotator SpawnRotation = Owner->GetActorRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Owner;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AActor* SpawnActor = GetWorld()->SpawnActor<ACItemBase>(Item, SpawnLocation, SpawnRotation, SpawnParams);

	if (SpawnActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnSucceeded"));
	}

}

