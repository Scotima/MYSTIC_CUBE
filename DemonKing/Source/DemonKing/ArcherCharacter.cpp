#include "ArcherCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h"
#include "Components/InputComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "DemonKing/ActorComponent/PlayerComponent/CCharacterStatComponent.h"

AArcherCharacter::AArcherCharacter()
{
}

void AArcherCharacter::BeginArrowRainTargeting()
{
    // 기존 평타의 홀드·연사를 종료
    SetUsingSkill(false);
    ServerSkillInput(0, false);
    bIsAimingArrowRain = true;

    UE_LOG(LogTemp, Log, TEXT("ArrowRain: Targeting started"));
}

void AArcherCharacter::CancelArrowRainTargeting()
{
    bIsAimingArrowRain = false;

    UE_LOG(LogTemp, Log, TEXT("ArrowRain: Targeting cancelled"));
}

void AArcherCharacter::InputSkillE()
{
    if (!IsLocallyControlled())
    {
        return;
    }

    if (bIsAimingArrowRain)
    {
        ConfirmArrowRainTargeting();
    }
    else
    {
        BeginArrowRainTargeting();
    }
}

void AArcherCharacter::ConfirmArrowRainTargeting()
{
    if (!IsLocallyControlled() || !bIsAimingArrowRain)
    {
        return;
    }

    // 확정 버튼을 누른 순간의 위치를 다시 계산
    UpdateArrowRainTargeting();

    if (!bHasValidArrowRainTarget)
    {
        UE_LOG(LogTemp, Warning,
            TEXT("ArrowRain: Invalid target"));
        return;
    }

    ConfirmedArrowRainLocation = ArrowRainTargetLocation;

    bIsAimingArrowRain = false;
    bHasValidArrowRainTarget = false;

    UE_LOG(LogTemp, Log,
        TEXT("ArrowRain: Target confirmed at %s"),
        *ConfirmedArrowRainLocation.ToString());
    ServerCastArrowRain(ConfirmedArrowRainLocation);
}

void AArcherCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (IsLocallyControlled() && bIsAimingArrowRain)
    {
        UpdateArrowRainTargeting();
    }
}

void AArcherCharacter::UpdateArrowRainTargeting()
{
    bHasValidArrowRainTarget = false;

    APlayerController* PC = Cast<APlayerController>(GetController());
    UWorld* World = GetWorld();

    if (!PC || !World)
    {
        return;
    }

    FVector CameraLocation;
    FRotator CameraRotation;
    PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

    const FVector TraceEnd =
        CameraLocation + CameraRotation.Vector() * 10000.0f;

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);

    // 우선 정적 지형을 대상으로 조준
    FCollisionObjectQueryParams ObjectParams;
    ObjectParams.AddObjectTypesToQuery(ECC_WorldStatic);

    const FVector ActorLocation = GetActorLocation();

    // 카메라가 바라보는 지점을 먼저 찾기
    FHitResult CameraHit;
    const bool bCameraHit = World->LineTraceSingleByObjectType(
        CameraHit, CameraLocation, TraceEnd, ObjectParams, QueryParams);

    // 지면을 못 찾으면 바라보는 방향의 최대 사거리를 사용
    FVector TargetDirection =
        CameraRotation.Vector().GetSafeNormal2D();

    float TargetDistance = SkillECastRange;

    if (bCameraHit)
    {
        FVector ToHit = CameraHit.ImpactPoint - ActorLocation;
        ToHit.Z = 0.0f;

        if (FVector::DotProduct(ToHit, TargetDirection) > 0.0f)
        {
            TargetDirection = ToHit.GetSafeNormal();

            TargetDistance =
                ToHit.Size2D() * ArrowRainDistanceScale;
        }
        else
        {
            // 카메라가 캐릭터 뒤쪽 바닥을 보고 있으면 발밑으로 제한
            TargetDistance = 0.0f;
        }
    }

    // 거리가 늘어나도 최대 사거리를 넘지 않게 함
    TargetDistance = FMath::Clamp(
        TargetDistance, 0.0f, FMath::Max(0.0f, SkillECastRange));

    const FVector CandidateLocation =
        ActorLocation + TargetDirection * TargetDistance;

    // 보정된 위치에서 지면 다시 탐색
    // 카메라로 높은 지형을 보고 있었다면 검사 시작 높이도 올림
    const double ProbeHeight = bCameraHit
        ? FMath::Max(ActorLocation.Z, CameraHit.ImpactPoint.Z)
        : ActorLocation.Z;

    const FVector GroundStart(
        CandidateLocation.X, CandidateLocation.Y, ProbeHeight + 500.0);

    const FVector GroundEnd(
        CandidateLocation.X, CandidateLocation.Y, ActorLocation.Z - 2000.0);

    FHitResult Hit;
    if (!World->LineTraceSingleByObjectType(
        Hit, GroundStart, GroundEnd, ObjectParams, QueryParams))
    {
        return;
    }

    ArrowRainTargetLocation = Hit.ImpactPoint;

    const bool bGroundSurface = Hit.ImpactNormal.Z >= 0.5f;

    const bool bWithinRange =
        FVector::Dist2D(ActorLocation, ArrowRainTargetLocation)
        <= SkillECastRange + 1.0f;

    // 거리 배율 때문에 벽 너머로 넘어간 위치는 사용할 수 없음
    FHitResult ObstacleHit;
    const bool bBlocked = World->LineTraceSingleByObjectType(
        ObstacleHit,
        ActorLocation + FVector(0.0, 0.0, 50.0),
        ArrowRainTargetLocation + FVector(0.0, 0.0, 50.0),
        ObjectParams,
        QueryParams);

    bHasValidArrowRainTarget =
        bGroundSurface && bWithinRange && !bBlocked;

    // 맞은 표면 방향에 맞춰 원 그림
    FVector AxisX;
    FVector AxisY;
    Hit.ImpactNormal.FindBestAxisVectors(AxisX, AxisY);

    DrawDebugCircle(
        World,
        ArrowRainTargetLocation + Hit.ImpactNormal * 5.0f,
        ArrowRainRadius,
        64,
        bHasValidArrowRainTarget ? FColor::Green : FColor::Red,
        false,
        -1.0f,
        0,
        2.0f,
        AxisX,
        AxisY,
        false
    );
}

void AArcherCharacter::SetupPlayerInputComponent(
    UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // 부모가 등록한 궁수의 E 입력만 제거
    // 컨트롤러의 OnEPressed()에서 InputSkillE()를 호출
    PlayerInputComponent->RemoveActionBinding(
        FName(TEXT("SkillE")), IE_Pressed);
}

void AArcherCharacter::ServerCastArrowRain_Implementation(
    FVector TargetLocation)
{
    UWorld* World = GetWorld();

    UCCharacterStatComponent* Stat =
        FindComponentByClass<UCCharacterStatComponent>();

    const auto RejectCast = [this]()
        {
            ClientArrowRainCastRejected();
        };

    // 서버에서 생존, 쿨다운, 마나를 확인
    if (!World || !IsValid(Stat) ||
        Stat->GetCurrentHp() <= 0.0f ||
        !CanUseSkill(ESkillSlot::E) ||
        TargetLocation.ContainsNaN())
    {
        RejectCast();
        return;
    }

    const FVector ActorLocation = GetActorLocation();

    if (FVector::Dist2D(ActorLocation, TargetLocation)
        > SkillECastRange + 1.0f)
    {
        RejectCast();
        return;
    }

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);

    FCollisionObjectQueryParams ObjectParams;
    ObjectParams.AddObjectTypesToQuery(ECC_WorldStatic);

    // 전달받은 위치에 실제 지면이 있는지 다시 확인
    FHitResult GroundHit;
    const bool bGroundHit = World->LineTraceSingleByObjectType(
        GroundHit,
        TargetLocation + FVector(0.0, 0.0, 50.0),
        TargetLocation - FVector(0.0, 0.0, 50.0),
        ObjectParams,
        QueryParams);

    if (!bGroundHit || GroundHit.ImpactNormal.Z < 0.5f)
    {
        RejectCast();
        return;
    }

    // 캐릭터와 설치 위치 사이의 장애물 확인
    FHitResult ObstacleHit;
    const bool bBlocked = World->LineTraceSingleByObjectType(
        ObstacleHit,
        ActorLocation + FVector(0.0, 0.0, 50.0),
        GroundHit.ImpactPoint + FVector(0.0, 0.0, 50.0),
        ObjectParams,
        QueryParams);

    if (bBlocked)
    {
        RejectCast();
        return;
    }

    // 기존 BP가 읽는 E 목표 위치에 승인된 위치 저장
    CachedSkillETargetLocation = GroundHit.ImpactPoint;

    // 이펙트 생성까지 다른 스킬의 중복 시전 막음
    bIsCastingSkill = true;

    // 승인된 경우에만 마나와 쿨다운 소비
    CommitSkill(ESkillSlot::E);

    // 기존 BP의 몽타주 → Delay 흐름 실행
    BP_UseSkillE();
}

void AArcherCharacter::ClientArrowRainCastRejected_Implementation()
{
    UE_LOG(LogTemp, Warning,
        TEXT("ArrowRain: Server rejected cast"));
}

void AArcherCharacter::MulticastSpawnArrowRainEffect_Implementation(
    UNiagaraSystem* Effect,
    FVector TargetLocation)
{
    if (HasAuthority())
    {
        bIsCastingSkill = false;
    }

    // 전용 서버에서는 화면용 이펙트가 필요하지 않습니다.
    if (!Effect || GetNetMode() == NM_DedicatedServer)
    {
        return;
    }

    UNiagaraFunctionLibrary::SpawnSystemAtLocation(
        GetWorld(),
        Effect,
        TargetLocation,
        FRotator::ZeroRotator,
        FVector(1.0f),
        true,
        true);
}