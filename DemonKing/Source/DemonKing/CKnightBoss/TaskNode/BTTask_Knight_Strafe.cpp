// Fill out your copyright notice in the Description page of Project Settings.
#include "DemonKing/CKnightBoss/TaskNode/BTTask_Knight_Strafe.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"
#include "BrainComponent.h"
#include "Navigation/PathFollowingComponent.h"

EBTNodeResult::Type UBTTask_Knight_Strafe::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

    AAIController* AIController = OwnerComp.GetAIOwner();

    APawn* BossPawn = AIController ? AIController->GetPawn() : nullptr;

    if (!IsValid(BossPawn))
    {
		return EBTNodeResult::Failed;

    }

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

    if (!IsValid(BlackboardComp))
    {
        return EBTNodeResult::Failed;
    }

    AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TEXT("TargetActor")));

    if (!IsValid(TargetActor))
    {
		return EBTNodeResult::Failed;
    }

    FVector BossLocation = BossPawn->GetActorLocation();
    FVector TargetLocation = TargetActor->GetActorLocation();

    AIController->SetFocus(TargetActor);

    float AngleDegrees = FMath::RandBool() ? 60.0f : -60.0f;

    FVector Destination = CaculateLookYaw(BossLocation, TargetLocation, AngleDegrees);

    const EPathFollowingRequestResult::Type Result = AIController->
        MoveToLocation(Destination, 10.0f, false, true, true, true,
            nullptr, false);


    if (Result == EPathFollowingRequestResult::Failed)
    {
        return EBTNodeResult::Failed;
    }
   

    if(Result == EPathFollowingRequestResult::AlreadyAtGoal)
    {
        return EBTNodeResult::Succeeded;
    }

    WaitForMessage(
        OwnerComp,
        UBrainComponent::AIMessage_MoveFinished,
        AIController->GetCurrentMoveRequestID()
    );

    // 이동 도중 경로를 다시 찾다가 실패하는 경우도 처리해요.
    WaitForMessage(
        OwnerComp,
        UBrainComponent::AIMessage_RepathFailed
    );
    

    return EBTNodeResult::InProgress;

}

FVector UBTTask_Knight_Strafe::CaculateLookYaw(
    const FVector& BossLocation, const FVector& TargetLocation, float AngleDgrees) const
{
    const float OrbitRadius = 1000.0f;

    const FVector DirectionFromTarget =
		(BossLocation - TargetLocation).GetSafeNormal2D();

    const FVector RotatedDirection =
		DirectionFromTarget.RotateAngleAxis(AngleDgrees, FVector::UpVector);

	FVector Destination = TargetLocation + RotatedDirection * OrbitRadius;

    Destination.Z = BossLocation.Z;

    return Destination;


    
}
