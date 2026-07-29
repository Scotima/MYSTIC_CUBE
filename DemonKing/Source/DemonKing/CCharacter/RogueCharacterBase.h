// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Materials/MaterialInterface.h"
#include "RogueCharacterBase.generated.h"

UCLASS()
class DEMONKING_API ARogueCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARogueCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:
	virtual void InputSkillQ();
	virtual void InputSkillE();
	virtual void InputSkillShift();

protected:
	UPROPERTY(Transient)
		TObjectPtr<class UCameraComponent> OcclusionCamera;

	UPROPERTY(Transient)
	TObjectPtr<class USpringArmComponent> OcclusionSpringArm;

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void UpdateCameraOcclusionFade();
	void FadeOccludingMesh(class UMeshComponent* MeshComponent);
	void RestoreOccludingMesh(class UMeshComponent* MeshComponent);
	void RestoreAllOccludingMeshes();
	bool IsValidOcclusionTarget(const class UMeshComponent* MeshComponent) const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera|Occlusion")
	bool bEnableCameraOcclusionFade = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera|Occlusion")
	float CameraOcclusionTraceRadius = 24.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera|Occlusion")
	float CameraOcclusionFadeOpacity = 0.35f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera|Occlusion")
	int32 CameraOcclusionMaxTraceCount = 16;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera|Occlusion")
	TEnumAsByte<ECollisionChannel> CameraOcclusionTraceChannel = ECC_Visibility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera|Occlusion")
	TObjectPtr<UMaterialInterface> CameraOcclusionFadeMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera|Occlusion")
	FName CameraOcclusionOpacityParameter = TEXT("Opacity");

	TMap<class UMeshComponent*, TArray<TObjectPtr<UMaterialInterface>>> FadedOccluderMaterials;

};
