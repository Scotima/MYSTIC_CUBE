#include "DemonKing/ActorComponent/EnemyComponent/CEnemyStatComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/EngineTypes.h"
#include "DemonKing/ActorComponent/PlayerComponent/CCharacterStatComponent.h"
#include "DemonKing/Item/CItemBase.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
UCEnemyStatComponent::UCEnemyStatComponent()
{

	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);


	MaxHp = 100.0f;
	Defense = 100.0f;
	CurrentHp = MaxHp;


	
}
//함수 체력 받고 방어력 받으면
//* / 개지랄 염병을 떨어서 리턴을 하겠지 함수를 flaot형으로 처리를 할 예정.,


void UCEnemyStatComponent::BeginPlay()
{
	Super::BeginPlay();


}



void UCEnemyStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

void UCEnemyStatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCEnemyStatComponent, CurrentHp);
}

void UCEnemyStatComponent::AttackPlayer(AActor* HitActor)
{

	if (!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("[UCEnemyStatComponent]"
			"::AttackPlayer !GetOwner->HasAuthority"));
		return;
	}

	UCCharacterStatComponent* Player = HitActor->FindComponentByClass<UCCharacterStatComponent>();

	if (!Player)
	{
		return;
	}

	Player->TakeDamage(AttackPower);
}

void UCEnemyStatComponent::TakeDamage(float PlayerPower,float DefensePenetration, float DefenseIgnoreRate)
{
	if (CurrentHp <= 0)
	{
		return; //Aready Hp is Zero return
	}

	DefenseIgnoreRate = FMath::Clamp(DefenseIgnoreRate, 0.0f, 1.0f);

	float EffectiveDefense = Defense - DefensePenetration;
	EffectiveDefense = FMath::Max(0.0f, EffectiveDefense);
	
	EffectiveDefense *= (1.0f - DefenseIgnoreRate);

	const float DamageRate = 100.0f / (100.0f + EffectiveDefense);
	const float FinalDamage = FMath::Max(1.0f, PlayerPower * DamageRate);

	CurrentHp = FMath::Clamp(CurrentHp - FinalDamage, 0.0f, MaxHp);
	OnEmenyHpChanged.Broadcast(GetHealthPercent());
	
	
	if (CurrentHp <= 0.0f)
	{
		Die();
	}

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

			ServerAttackPlayer(HitActor);
			
		}
	}
}

void UCEnemyStatComponent::Die()
{
	//call function Enemy->PlayMontage and Destroy this;
	AActor* Owner = GetOwner();

	if (!IsValid(Owner))
	{
		return;
	}


	OnEnemyDied.Broadcast();
	Owner->Destroy();
	DropItem();
	
}

void UCEnemyStatComponent::ServerAttackPlayer_Implementation(AActor* HitActor)
{
	AttackPlayer(HitActor);
}

void UCEnemyStatComponent::OnRep_Current()
{
	OnEmenyHpChanged.Broadcast(GetHealthPercent());
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

	FActorSpawnParameters SpawnParams; // 만약 스폰하려는 위치에 뭔가 충돌할만한게 있으면 알아서 수정.
	SpawnParams.Owner = Owner;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AActor* SpawnActor = GetWorld()->SpawnActor<ACItemBase>(Item, SpawnLocation, SpawnRotation, SpawnParams);

	if (SpawnActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnSucceeded"));
	}

}

