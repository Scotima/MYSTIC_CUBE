// Fill out your copyright notice in the Description page of Project Settings.


#include "DemonKing/FoxGirl/CKatanaComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "DemonKing/ActorComponent/EnemyComponent/CEnemyStatComponent.h"
#include "DemonKing/CCharacter/RogueCharacterBase.h"

// Sets default values
ACKatanaComponent::ACKatanaComponent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	WeaponMesh->SetSimulatePhysics(false); // 물리 충돌 꺼주기/.
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(WeaponMesh);

	CollisionBox->SetBoxExtent(FVector(10.0f, 10.f, 50.f));
	CollisionBox->SetGenerateOverlapEvents(true);

	CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);


}

// Called when the game starts or when spawned
void ACKatanaComponent::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority() && IsValid(CollisionBox))
	{
		CollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ACKatanaComponent::OnAttackBoxOverlap);
	}

}

// Called every frame
void ACKatanaComponent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACKatanaComponent::SetAttackCollisionEnabled(bool bEnabled)
{
	UE_LOG(LogTemp, Warning,
		TEXT("[Katana] 요청 | Server=%d | Enable=%d"),
		HasAuthority(), bEnabled);


	if (!HasAuthority() || !IsValid(CollisionBox))
	{
		return;
	}

	CollisionBox->SetCollisionEnabled(bEnabled ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);

	UE_LOG(LogTemp, Warning,
		TEXT("[Katana] CollisionEnabled=%d"),
		static_cast<int32>(CollisionBox->GetCollisionEnabled()));
}

void ACKatanaComponent::OnAttackBoxOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIdex, bool bFromSweep, const FHitResult & SweepResult)
{

	UE_LOG(LogTemp, Warning,
		TEXT("[Katana Overlap] Actor=%s | Component=%s"),
		*GetNameSafe(OtherActor),
		*GetNameSafe(OtherComp));


	if (!HasAuthority())
	{
		return;
	}

	if (!IsValid(OtherActor) || OtherActor == this || OtherActor->IsA<ARogueCharacterBase>())
	{
		return;
	}

	UCEnemyStatComponent* EnemyStatComp = OtherActor->FindComponentByClass<UCEnemyStatComponent>();

	if (!IsValid(EnemyStatComp))
	{
		return;
	}

	EnemyStatComp->TakeDamage(10.0f);

	UE_LOG(LogTemp, Warning,
		TEXT("[KatanaDamage] Target=%s"),
		*GetNameSafe(OtherActor));

	

}


