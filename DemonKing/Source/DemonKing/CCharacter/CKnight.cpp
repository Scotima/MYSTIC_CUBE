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
	UE_LOG(LogTemp, Warning, TEXT("[ACKnight::InputSkillLeftMouse]"));
	if(eSkillName != SkillName::NormalAttack)
	{	
		UE_LOG(LogTemp, Warning, TEXT("eSkillName != SkillName::NormalAttack"));
		eSkillName = SkillName::NormalAttack;
		ComboIndex = 0;
		bCanComboInput = false;
	}
	if(eSkillName == SkillName::NormalAttack)
	{
		if(bCanComboInput)
		{
			ComboIndex++;
			bCanComboInput = false;

		}
	    KnightSkillComponent->UseSkill(1000, ComboIndex);
		
	}
	
		
}

void ACKnight::InputSkillQ()
{
	if(eSkillName != SkillName::SkillQ)
	{
		eSkillName = SkillName::SkillQ;
		ComboIndex = 0;
		bCanComboInput = false;
	}

	if(eSkillName == SkillName::SkillQ)
	{
		if(bCanComboInput)
		{
			ComboIndex++;
			bCanComboInput = false;
		}
		KnightSkillComponent->UseSkill(2000, ComboIndex);
	}
}

void ACKnight::InputSkillE()
{
	if(eSkillName != SkillName::SkillE)
	{
		eSkillName = SkillName::SkillE;
		ComboIndex = 0;
		bCanComboInput = false;
	}

	if(eSkillName == SkillName::SkillE)
	{
		if(bCanComboInput)
		{
			ComboIndex++;
			bCanComboInput = false;
		}

		KnightSkillComponent->UseSkill(3000, ComboIndex);
	}

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

	FVector SpawnLocation = BaseTrans.TransformPosition(LocationOffset);//���� ��ġ �������� ��ġ����.
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


