#include "DemonKing/SkillComponent/CKnightSkillComponent.h"
#include "TimerManager.h"
#include "DemonKing/CCharacter/CKnight.h"
#include "Animation/AnimMontage.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/EngineTypes.h"
#include "DemonKing/ActorComponent/EnemyComponent/CEnemyStatComponent.h"
#include "DemonKing/ActorComponent/PlayerComponent/CCharacterStatComponent.h"

UCKnightSkillComponent::UCKnightSkillComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

	bUseSkill = true;
	MotionEnd = true;
}


void UCKnightSkillComponent::BeginPlay()
{
	Super::BeginPlay();

	
	
}


void UCKnightSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

void UCKnightSkillComponent::DoTrace(const FBoxTraceData& BoxTraceData)
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

	

	const FVector Start = Owner->GetActorLocation() + TraceDir*BoxTraceData.StartDistance;
	const FVector End = Start + TraceDir * TraceDistance;

	TArray<FHitResult> HitResults;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(Owner);

	bool Hit = UKismetSystemLibrary::BoxTraceMulti(this,
		Start, End, BoxHalfSize, TraceDir.Rotation(), UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false, IgnoreActors, EDrawDebugTrace::ForDuration, HitResults, true);


	if (Hit)
	{
		UE_LOG(LogTemp, Warning, TEXT("HIT"));

		TSet<AActor*> AlreadyHitActors; // HashAlgorithm

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

			UCEnemyStatComponent* Enemy = HitActor->FindComponentByClass<UCEnemyStatComponent>();

			if (!Enemy)
			{
				continue;
			}

			UCCharacterStatComponent* Stat = Owner->FindComponentByClass<UCCharacterStatComponent>();
			if (!Stat)
			{
				continue;
			}

			float Damage = 0.0f;
			if (bCurrentSkillIsBasicAttack)
			{
				bool bIsCrit = false;
				Damage = Stat->CalculateBasicAttackDamage(bIsCrit);
			}
			else
			{
				Damage = Stat->CalculateSkillDamage(CurrentSkillDamageCoefficient);
			}

			Enemy->TakeDamage(Damage);
		}
	}




}

void UCKnightSkillComponent::UseSkill(int SkillID, int32 ComboIndex)
{
	

	if ((BeforeSKillId / 1000 % 10)  != (SkillID / 1000 % 10))
	{
		BeforeSKillId = SkillID;
		bUseSkill = true;
	}

	
	SkillID += ComboIndex;
	FName SkillName = FName(*FString::FromInt(SkillID));

	FCharacterSkillStruct* SkillData = GetSkillDataTable(SkillName);

	if (!SkillData)
	{
		return;
	}

	UAnimMontage* animMontage = SkillData->Montage;
	float coolTime = SkillData->CoolDown;
	CurrentSkillDamageCoefficient = SkillData->Damage;
	bCurrentSkillIsBasicAttack = (SkillID / 1000) == 1;

	UCCharacterStatComponent* Stat = GetOwner() ? GetOwner()->FindComponentByClass<UCCharacterStatComponent>() : nullptr;
	if (Stat)
	{
		coolTime = bCurrentSkillIsBasicAttack ? Stat->GetAttackInterval() : Stat->GetFinalCooldown(coolTime);
	}

	
	
 
 	
	if (!animMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UCKnightSkillComponent::UseSkill] animMontage is nullptr"));
		return;
	}
	
	if (CoolDownSystem(SkillID)&& MotionEnd)
	{
		MotionEnd = false;
		OwnerCharacter = Cast<ACKnight>(GetOwner());

		if (OwnerCharacter)
		{
			if (bCanInput == true)
			{
				bCanInput = false;
				OwnerCharacter->GetController()->SetIgnoreMoveInput(true);
			}
			UE_LOG(LogTemp, Warning, TEXT("[UCKnightSkillComponent::UseSkill] UseSKill"));
			OwnerCharacter->PlaySkillMotion(animMontage);
			float WorldTime =GetWorld()->GetTimeSeconds();
 			float EndSkillTime = WorldTime + coolTime;
			SkillCoolTimeMap.Add(SkillID, EndSkillTime); 
			

		}
	}

	else{
		UE_LOG(LogTemp, Warning, TEXT("[UCKnightSkillComponent::UseSkill] Skill is on CoolDown"));
	}
}

FCharacterSkillStruct* UCKnightSkillComponent::GetSkillDataTable(FName rowname)
{
	if (!SkillDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UCKnightSkillComponent::GetSkillDataTable] No SkillDataTable Please Input SkillDataTable"));
		return nullptr;
	}

	FCharacterSkillStruct* Skilldata = SkillDataTable->FindRow<FCharacterSkillStruct>(rowname, TEXT("GetSkillData"));

	return Skilldata;
}

bool UCKnightSkillComponent::CoolDownSystem(int SkillID)
{
 	float WorldTime = GetWorld()->GetTimeSeconds();
 	float* EndCool = SkillCoolTimeMap.Find(SkillID);
	
	if(!EndCool)
	{
		return true;
	}

	return WorldTime >= *EndCool;
	
}

