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
			OwnerCharacter->PlaySkillMotion(animMontage); // 스킬 애니메이션 구현.
			GetWorld()->GetTimerManager().SetTimer(timerhandle, this, &UCKnightSkillComponent::CoolDownSystem, coolTime, false, 1.0);
			

		}
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
	GetWorld()->GetTimerManager().ClearTimer(timerhandle);
	bUseSkill = true;
}

