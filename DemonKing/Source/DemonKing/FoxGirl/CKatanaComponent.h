// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CKatanaComponent.generated.h"

UCLASS()
class DEMONKING_API ACKatanaComponent : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACKatanaComponent();



	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SetAttackCollisionEnabled(bool bEnabled);


public:
	UFUNCTION()
	void OnAttackBoxOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIdex,
		bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<class UStaticMeshComponent> WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<class UBoxComponent> CollisionBox;

};
