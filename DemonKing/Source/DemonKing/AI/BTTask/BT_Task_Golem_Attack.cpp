#include "DemonKing/AI/BTTask/BT_Task_Golem_Attack.h"
#include "DemonKing/Enemy/CEnemyBase.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"

EBTNodeResult::Type UBT_Task_Golem_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACEnemyBase* EnemyCharacter = Cast<ACEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());

	if (!IsValid(EnemyCharacter))
	{
		return EBTNodeResult::Failed;
	
	}


	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (!IsValid(Blackboard))
	{
		UE_LOG(LogTemp, Warning, TEXT("[EBTNodeResult::Type UBT_Task_Golem_Attack] :: ExecuteTask() "
			"!IsValid Blackboard"));
		return EBTNodeResult::Failed;

	}

	AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(TEXT("TargetActor")));

	if (!IsValid(TargetActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("[EBTNodeResult::Type UBT_Task_Golem_Attack] :: ExecuteTask"
			"!IsValid TargetActor"));
		return EBTNodeResult::Failed;
	}

	constexpr float AttackRange = 250.0f;

	const float DistanceSquared = FVector::DistSquared2D(
		EnemyCharacter->GetActorLocation(), TargetActor->GetActorLocation());

	if (DistanceSquared > FMath::Square(AttackRange)) //Square는 제곱하는 함수 거리라서 DistSquared도 제곱한 값이라.
	{
		return EBTNodeResult::Failed;
	}

	float PlayDuration = EnemyCharacter->Play_AnimMontage();

	if (PlayDuration <= 0.0f)
	{
		return EBTNodeResult::Failed;
	}

	UAnimInstance* AnimInstance = EnemyCharacter->GetMesh()->GetAnimInstance();

	if (!IsValid(AnimInstance))
	{
		return EBTNodeResult::Failed;
	}

	UAnimMontage* ActiveMontage = AnimInstance->GetCurrentActiveMontage();


	TWeakObjectPtr<UBehaviorTreeComponent> WeakOwnerComp(&OwnerComp); // 콜백시 컴포넌트가 없어져 비정상적으로 종료될 가능성을 염려.

	FOnMontageEnded EndDelegate;

	EndDelegate.BindLambda([this, WeakOwnerComp](UAnimMontage* Montage, bool bInerrupted) {
		if (!WeakOwnerComp.IsValid())
		{
			return;
		}

		FinishLatentTask(*WeakOwnerComp.Get(), bInerrupted ? EBTNodeResult::Failed : EBTNodeResult::Succeeded);


		});


	AnimInstance->Montage_SetEndDelegate(EndDelegate, ActiveMontage);

	return EBTNodeResult::InProgress;
}
