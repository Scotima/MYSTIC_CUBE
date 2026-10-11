// Fill out your copyright notice in the Description page of Project Settings.


#include "DemonKing/FoxGirl/CKatanaComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ACKatanaComponent::ACKatanaComponent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	WeaponMesh->SetSimulatePhysics(false); // 물리 충돌 꺼주기/.
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);


}

// Called when the game starts or when spawned
void ACKatanaComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACKatanaComponent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

