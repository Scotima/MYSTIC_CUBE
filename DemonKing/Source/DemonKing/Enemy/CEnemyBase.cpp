#include "DemonKing/Enemy/CEnemyBase.h"
#include "Animation/AnimInstance.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

ACEnemyBase::ACEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

}

// Called when the game starts or when spawned
void ACEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ACEnemyBase::Play_AnimMontage()
{
	if (!IsValid(AttackAnim))
	{
		UE_LOG(LogTemp, Warning, TEXT("[ACEnemyBase] :: Play_AnimMontage -> !IsValid AttackAnim"));
		return 0.0f;
	}

	USkeletalMeshComponent* SMC = GetMesh();

	if (!SMC)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ACEnemyBase] :: Play_AnimMontage -> !SMC"));
		return 0.0f;
	}

	UAnimInstance* Anim_Ins = SMC->GetAnimInstance();

	if (!Anim_Ins)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ACEnemyBase] :: Play_AnimMontage -> !Anim_Ins"));
		return 0.0f;
	}


	return Anim_Ins->Montage_Play(AttackAnim);
}

