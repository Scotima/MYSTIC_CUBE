#include "DemonKing/Enemy/CEnemyBase.h"
#include "Animation/AnimInstance.h"

ACEnemyBase::ACEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

void ACEnemyBase::Play_AnimMontage()
{

}

