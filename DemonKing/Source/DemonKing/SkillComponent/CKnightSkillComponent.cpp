#include "DemonKing/SkillComponent/CKnightSkillComponent.h"
#include "TimerManager.h"
#include "DemonKing/CCharacter/CKnight.h"


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

void UCKnightSkillComponent::UseSkill(FName skillname)
{
	float coolTime = GetSkillDataTable(skillname)->CoolDown;

	if (bUseSkill)
	{
		OwnerCharacter = Cast<ACKnight>(GetOwner());

		if (OwnerCharacter)
		{
			GetWorld()->GetTimerManager().SetTimer(timerhandle, this, &UCKnightSkillComponent::CoolDownSystem, coolTime, false, 1.0);
			//스킬 애니메이션 구현.
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

