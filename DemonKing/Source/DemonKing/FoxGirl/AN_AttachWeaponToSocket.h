// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_AttachWeaponToSocket.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FWeaponAttachSettings
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    FName ComponentTag = TEXT("Katana");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    FName SocketName = TEXT("Huri_Socket");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    FVector LocationOffset = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    FRotator RotationOffset = FRotator::ZeroRotator;
};

UCLASS()
class DEMONKING_API UAN_AttachWeaponToSocket : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(
    USkeletalMeshComponent* MeshComp,
    UAnimSequenceBase* Animation,
    const FAnimNotifyEventReference& EventReference) override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FWeaponAttachSettings AttachSettings;


	
};
