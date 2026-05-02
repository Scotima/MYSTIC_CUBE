#include "DemonKing/CCharacter/CKnight.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "DemonKing/SkillComponent/CKnightSkillComponent.h"
ACKnight::ACKnight()
{
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	KnightSkillComponent = CreateDefaultSubobject<UCKnightSkillComponent>(TEXT("KnightSkillComponent"));

	
	
	WeaponMesh->SetupAttachment(GetMesh(), WeaponSocketName);

	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetGenerateOverlapEvents(false);

	ComboIndex = 0;


}

void ACKnight::StartNiagaraImpact(UNiagaraSystem* niagaraSystem, FName Socketname)
{
	NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(niagaraSystem, WeaponMesh, Socketname, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, false);



	if (NiagaraComponent)
	{
		NiagaraComponent->Activate(true);
	}

}

void ACKnight::EndNiagaraImpact()
{
	if (!NiagaraComponent)
	{
		return;
	}

	NiagaraComponent->Deactivate();
	NiagaraComponent->DestroyComponent();
	NiagaraComponent = nullptr;
}

void ACKnight::InputSkillLeftMouse()
{
	
	KnightSkillComponent->UseSkill(1000, ComboIndex);
	//todo 콤보인덱스 공유 문제와 콤보 인덱스 증가 문제를 고민해볼것.
}

void ACKnight::InputSkillQ()
{
	if (eSkillName != SkillName::SkillQ)
	{
		eSkillName = SkillName::SkillQ;
		ComboIndex = 0;
	}

	if (bCanComboInput && ComboIndex < 1)
	{
		ComboIndex++;

	}

	//다른 스킬을 쓰고 여기 스킬을 쓰려고 했을 때 콤보박스를 공유해버리면 이상한데서 1타를 쌓고 여기에서 2타가 되버리는데..
	
	KnightSkillComponent->UseSkill(2000, ComboIndex);



}

void ACKnight::InputSkillE()
{
	if (eSkillName != SkillName::SkillE)
	{
		eSkillName = SkillName::SkillE;
		ComboIndex = 0;
	}

	if (bCanComboInput && ComboIndex < 1)
	{
		ComboIndex++;
	}

	KnightSkillComponent->UseSkill(3000, ComboIndex);
}


UNiagaraComponent* ACKnight::SpawnNiagaraSystem(UNiagaraSystem* niagarasystem, FName SocketName, FVector LocationOffset,
	FRotator RotationOffset, FVector Scale)
{
	if (!niagarasystem)
	{
		return nullptr;
	}

	UWorld* world = GetWorld();

	if (!world)
	{
		return nullptr;
	}

	USkeletalMeshComponent* meshComp = GetMesh();

	if (!meshComp)
	{
		return nullptr;
	}

	const FTransform BaseTrans = SocketName != NAME_None ? meshComp->GetSocketTransform(SocketName, RTS_World) : meshComp->GetComponentTransform();

	FVector SpawnLocation = BaseTrans.TransformPosition(LocationOffset);//소켓 위치 기준으로 위치조정.
	FQuat SpawnQut = BaseTrans.GetRotation() * RotationOffset.Quaternion();
	FRotator SpawnRotation = SpawnQut.Rotator();



	FTransform KnightTransform = meshComp->GetComponentTransform();

	return UNiagaraFunctionLibrary::SpawnSystemAtLocation(world, niagarasystem, SpawnLocation, SpawnRotation, Scale, true, true);


}

float ACKnight::PlaySkillMotion(UAnimMontage* animmontage, float PlayRate)
{
	if (!animmontage)
	{
		return 0.0f;
	}

	USkeletalMeshComponent* meshComp = GetMesh();

	if (!meshComp)
	{
		return 0.0f;
	}

	UAnimInstance* animInstance = meshComp->GetAnimInstance();

	if (!animInstance)
	{
		return 0.0f;
	}

	return animInstance->Montage_Play(animmontage, PlayRate);


}


