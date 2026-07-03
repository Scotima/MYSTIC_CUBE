#include "DemonKing/SkillComponent/CKnightSkillComponent.h"
#include "TimerManager.h"
#include "DemonKing/CCharacter/CKnight.h"
#include "Animation/AnimMontage.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/EngineTypes.h"

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
	}




}

void UCKnightSkillComponent::UseSkill(int SkillID, int32 ComboIndex)
{
	

	if ((BeforeSKillId / 1000 % 10)  != (SkillID / 1000 % 10)) // 스킬의 앞자리만 비교해서 같은 스킬인지 아닌지 비교
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
	float SkillDamage = SkillData->Damage;

	
	
 
 	
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
			UE_LOG(LogTemp, Warning, TEXT("[UCKnightSkillComponent::UseSkill] UseSKill"));
			OwnerCharacter->PlaySkillMotion(animMontage); //즉시 애니메이션이 재생되는 문제.
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

