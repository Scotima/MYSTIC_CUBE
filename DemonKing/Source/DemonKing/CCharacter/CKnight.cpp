#include "DemonKing/CCharacter/CKnight.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"

ACKnight::ACKnight()
{
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));

	
	
	WeaponMesh->SetupAttachment(GetMesh(), WeaponSocketName);

	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetGenerateOverlapEvents(false);


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
