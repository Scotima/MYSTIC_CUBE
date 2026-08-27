#include "DemonKing/CCharacter/RogueCharacterBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/MeshComponent.h"
#include "Engine/World.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "DemonKing/GameFlow/RoguePlayerState.h"
#include "DemonKing/CWidget/CPlayerHPWidgetComponent.h"
#include "DemonKing/ActorComponent/PlayerComponent/CCharacterStatComponent.h"


ARogueCharacterBase::ARogueCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OcclusionSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	OcclusionSpringArm->SetupAttachment(RootComponent);

	OcclusionSpringArm->TargetArmLength = 600.f;
	OcclusionSpringArm->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	OcclusionSpringArm->bUsePawnControlRotation = false;

	OcclusionCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	OcclusionCamera->SetupAttachment(OcclusionSpringArm, USpringArmComponent::SocketName);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->bOrientRotationToMovement = false;
	}
	

}

// Called when the game starts or when spawned
void ARogueCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	OcclusionCamera = FindComponentByClass<UCameraComponent>();
	OcclusionSpringArm = FindComponentByClass<USpringArmComponent>();

	if (OcclusionSpringArm)
	{
		OcclusionSpringArm->bDoCollisionTest = false;
	}
}

void ARogueCharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	ARoguePlayerState* RPS = Cast<ARoguePlayerState>(GetPlayerState());

	if (!IsValid(RPS))
	{
		UE_LOG(LogTemp, Warning, TEXT("[ARogueCharacterBase] :: OnRep_PlayerState !RPS"));
		return;
	}

	UCPlayerHPWidgetComponent* PHPWidgetComp = FindComponentByClass<UCPlayerHPWidgetComponent>();

	if (!IsValid(PHPWidgetComp))
	{
		return;
	}

	RPS->OnPlayerHpChanged.RemoveAll(PHPWidgetComp);

	RPS->OnPlayerHpChanged.AddUObject(PHPWidgetComp, &UCPlayerHPWidgetComponent::UpdateHealthComponent);

	
	PHPWidgetComp->UpdateHealthComponent(RPS->GetPlayerState_HP());

	UE_LOG(LogTemp, Warning, TEXT("[ARogueCharacterBase] :: OnRep_PlayerState() BindComplete"));

}

void ARogueCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ARoguePlayerState* RPS = Cast<ARoguePlayerState>(GetPlayerState());

	if (!IsValid(RPS))
	{
		UE_LOG(LogTemp, Warning, TEXT("[ARogueCharacterBase] :: OnRep_PlayerState !RPS"));
		return;
	}

	UCPlayerHPWidgetComponent* PHPWidgetComp = FindComponentByClass<UCPlayerHPWidgetComponent>();

	if (!IsValid(PHPWidgetComp))
	{
		return;
	}

	RPS->OnPlayerHpChanged.RemoveAll(PHPWidgetComp);

	RPS->OnPlayerHpChanged.AddUObject(PHPWidgetComp, &UCPlayerHPWidgetComponent::UpdateHealthComponent);

	Refresh_HP();
	PHPWidgetComp->UpdateHealthComponent(RPS->GetPlayerState_HP());
	UE_LOG(LogTemp, Warning, TEXT("[ARogueCharacterBase] :: OnRep_PlayerState() BindComplet"));


}

void ARogueCharacterBase::Refresh_HP()
{
	//자식에서 재정의.
}

// Called every frame
void ARogueCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCameraOcclusionFade();
}

// Called to bind functionality to input
void ARogueCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ARogueCharacterBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RestoreAllOccludingMeshes();

	Super::EndPlay(EndPlayReason);
}

void ARogueCharacterBase::UpdateCameraOcclusionFade()
{
	if (!bEnableCameraOcclusionFade || !CameraOcclusionFadeMaterial || !OcclusionCamera)
	{
		RestoreAllOccludingMeshes();
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const FVector TraceStart = OcclusionCamera->GetComponentLocation();
	const FVector TraceEnd = GetActorLocation();

	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(CameraOcclusionFade), false);
	QueryParams.AddIgnoredActor(this);

	const FCollisionShape TraceShape = FCollisionShape::MakeSphere(CameraOcclusionTraceRadius);

	TSet<UMeshComponent*> CurrentOccluders;

	for (int32 TraceIndex = 0; TraceIndex < CameraOcclusionMaxTraceCount; ++TraceIndex)
	{
		FHitResult HitResult;

		const bool bHit = World->SweepSingleByChannel(
			HitResult,
			TraceStart,
			TraceEnd,
			FQuat::Identity,
			CameraOcclusionTraceChannel,
			TraceShape,
			QueryParams
		);

		if (!bHit)
		{
			break;
		}

		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		UMeshComponent* MeshComponent = Cast<UMeshComponent>(HitComponent);

		if (IsValidOcclusionTarget(MeshComponent))
		{
			CurrentOccluders.Add(MeshComponent);
			FadeOccludingMesh(MeshComponent);
		}

		if (HitComponent)
		{
			QueryParams.AddIgnoredComponent(HitComponent);
		}
		else if (AActor* HitActor = HitResult.GetActor())
		{
			QueryParams.AddIgnoredActor(HitActor);
		}
		else
		{
			break;
		}
	}

	TArray<UMeshComponent*> ComponentsToRestore;
	for (const TPair<UMeshComponent*, TArray<TObjectPtr<UMaterialInterface>>>& Pair : FadedOccluderMaterials)
	{
		if (!CurrentOccluders.Contains(Pair.Key))
		{
			ComponentsToRestore.Add(Pair.Key);
		}
	}

	for (UMeshComponent* MeshComponent : ComponentsToRestore)
	{
		RestoreOccludingMesh(MeshComponent);
	}
}

bool ARogueCharacterBase::IsValidOcclusionTarget(const UMeshComponent* MeshComponent) const
{
	if (!MeshComponent || MeshComponent->GetOwner() == this)
	{
		return false;
	}

	const ECollisionChannel ObjectType = MeshComponent->GetCollisionObjectType();
	return ObjectType == ECC_WorldStatic || ObjectType == ECC_WorldDynamic;
}

void ARogueCharacterBase::FadeOccludingMesh(UMeshComponent* MeshComponent)
{
	if (!MeshComponent || FadedOccluderMaterials.Contains(MeshComponent))
	{
		return;
	}

	const int32 MaterialCount = MeshComponent->GetNumMaterials();
	if (MaterialCount <= 0)
	{
		return;
	}

	TArray<TObjectPtr<UMaterialInterface>> OriginalMaterials;
	OriginalMaterials.Reserve(MaterialCount);

	for (int32 MaterialIndex = 0; MaterialIndex < MaterialCount; ++MaterialIndex)
	{
		OriginalMaterials.Add(MeshComponent->GetMaterial(MaterialIndex));

		UMaterialInstanceDynamic* FadeMaterialInstance =
			UMaterialInstanceDynamic::Create(CameraOcclusionFadeMaterial, this);

		if (FadeMaterialInstance)
		{
			FadeMaterialInstance->SetScalarParameterValue(
				CameraOcclusionOpacityParameter,
				CameraOcclusionFadeOpacity
			);

			MeshComponent->SetMaterial(MaterialIndex, FadeMaterialInstance);
		}
		else
		{
			MeshComponent->SetMaterial(MaterialIndex, CameraOcclusionFadeMaterial);
		}
	}

	FadedOccluderMaterials.Add(MeshComponent, MoveTemp(OriginalMaterials));
}

void ARogueCharacterBase::RestoreOccludingMesh(UMeshComponent* MeshComponent)
{
	if (!MeshComponent)
	{
		return;
	}

	TArray<TObjectPtr<UMaterialInterface>> OriginalMaterials;
	if (!FadedOccluderMaterials.RemoveAndCopyValue(MeshComponent, OriginalMaterials))
	{
		return;
	}

	for (int32 MaterialIndex = 0; MaterialIndex < OriginalMaterials.Num(); ++MaterialIndex)
	{
		MeshComponent->SetMaterial(MaterialIndex, OriginalMaterials[MaterialIndex]);
	}
}

void ARogueCharacterBase::RestoreAllOccludingMeshes()
{
	TArray<UMeshComponent*> ComponentsToRestore;
	FadedOccluderMaterials.GetKeys(ComponentsToRestore);

	for (UMeshComponent* MeshComponent : ComponentsToRestore)
	{
		RestoreOccludingMesh(MeshComponent);
	}

	FadedOccluderMaterials.Empty();
}

void ARogueCharacterBase::InputSkillQ()
{
	//자식에서 재정의
}


void ARogueCharacterBase::InputSkillE()
{
	//자식에서 재정의
}


void ARogueCharacterBase::InputSkillShift()
{
	//자식에서 재정의
}

void ARogueCharacterBase::InputSkillLeftMouse()
{
	//자식에서 재정의.
}


void ARogueCharacterBase::InputSkillLeftMouseReleased()
{
}