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



void UCKnightSkillComponent::UseSkill(int SkillID, int32 ComboIndex)
{
	
	if (!GetOwner() || !GetOwner()->HasAuthority())
	{
		return;
	}


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
			
			OwnerCharacter->MulticastPlaySkillMontage(animMontage);
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

