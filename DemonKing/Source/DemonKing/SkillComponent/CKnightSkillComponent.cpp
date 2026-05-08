#include "DemonKing/SkillComponent/CKnightSkillComponent.h"
#include "TimerManager.h"
#include "DemonKing/CCharacter/CKnight.h"
#include "Animation/AnimMontage.h"
#include "UObject/ConstructorHelpers.h"


UCKnightSkillComponent::UCKnightSkillComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

	bUseSkill = true;
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

	if (bUseSkill)
	{
		bUseSkill = false;
		OwnerCharacter = Cast<ACKnight>(GetOwner());

		if (OwnerCharacter)
		{
			UE_LOG(LogTemp, Warning, TEXT("[UCKnightSkillComponent::UseSkill] UseSKill"));
			OwnerCharacter->PlaySkillMotion(animMontage); // ��ų �ִϸ��̼� ����.
			GetWorld()->GetTimerManager().SetTimer(timerhandle, this, &UCKnightSkillComponent::CoolDownSystem, 1, false, coolTime);
			

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

void UCKnightSkillComponent::CoolDownSystem()
{
	//todo 쿨타임 공유 문제 해결해보기.
	GetWorld()->GetTimerManager().ClearTimer(timerhandle);
	bUseSkill = true;
}

