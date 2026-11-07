#include "DemonKing/FoxGirl/CFoxGirl.h"
#include "DemonKing/FoxGirl/FoxSkillQComponent.h"
#include "DemonKing/FoxGirl/FoxSkillEComponent.h"
#include "DemonKing/FoxGirl/FoxSkillShiftComponent.h"
#include "Components/ChildActorComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "DemonKing/FoxGirl/FoxSkillMouseLeftComponent.h"
#include "DemonKing/FoxGirl/CKatanaComponent.h"

#include "DemonKing/FoxGirl/RogueCharacterMovementComponent.h"
ACFoxGirl::ACFoxGirl(const FObjectInitializer& ObjectInitializer) : 
    Super(ObjectInitializer.SetDefaultSubobjectClass<URogueCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{


}
void ACFoxGirl::BeginPlay()
{
    Super::BeginPlay();

    TArray<UChildActorComponent*> ChildActorComponents;
    GetComponents<UChildActorComponent>(ChildActorComponents);

    for (UChildActorComponent* ChildComponent : ChildActorComponents)
    {
        if (!IsValid(ChildComponent))
        {
            continue;
        }

        KatanaComponent = Cast<ACKatanaComponent>(ChildComponent->GetChildActor());

        if (!IsValid(KatanaComponent))
        {
            continue;
        }

    }

}
void ACFoxGirl::OnSkillMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    if (!IsValid(KatanaComponent))
    {
        return;
    }

	KatanaComponent->SetAttackCollisionEnabled(false);

    Super::OnSkillMontageEnded(Montage, bInterrupted);

}
void ACFoxGirl::InputSkillQ()
{
    if(!HasAuthority())
    {
        return;
    }
    SetActorRotation(FRotator(0.f, GetControlRotation().Yaw, 0.f));
    UFoxSkillQComponent* QSkill = FindComponentByClass<UFoxSkillQComponent>();

    if(!IsValid(QSkill))
    {
        return;
    }

    QSkill->PlaySkillMontage();
    UE_LOG(LogTemp, Warning, TEXT("InputSkillQ"));
}

void ACFoxGirl::InputSkillE()
{
      if(!HasAuthority())
    {
        return;
    }
    SetActorRotation(FRotator(0.f, GetControlRotation().Yaw, 0.f));
    UFoxSkillEComponent* ESkill = FindComponentByClass<UFoxSkillEComponent>();

    if(!IsValid(ESkill))
    {
        return;
    }

    ESkill->PlaySkillMontage();
    UE_LOG(LogTemp, Warning, TEXT("InputSkillE"));
}

void ACFoxGirl::InputSkillShift()
{
      if(!HasAuthority())
    {
        return;
    }
    SetActorRotation(FRotator(0.f, GetControlRotation().Yaw, 0.f));
    UFoxSkillShiftComponent* ShiftSkill = FindComponentByClass<UFoxSkillShiftComponent>();

    if(!IsValid(ShiftSkill))
    {
        return;
    }

    ShiftSkill->PlaySkillMontage();
    UE_LOG(LogTemp, Warning, TEXT("InputSkillShift"));
}

void ACFoxGirl::InputSkillLeftMouse()
{
    if(!HasAuthority())
    {
        return;
    }
    SetActorRotation(FRotator(0.f, GetControlRotation().Yaw, 0.f));
    UFoxSkillMouseLeftComponent* MouseLeftSkill = 
        FindComponentByClass<UFoxSkillMouseLeftComponent>();

    if(!IsValid(MouseLeftSkill))
    {
        return;
    }

    MouseLeftSkill->PlaySkillMontage();
    UE_LOG(LogTemp, Warning, TEXT("InputMouseLeftSkill"));
}

bool ACFoxGirl::AttachKatanaToSocket(UChildActorComponent *InKatanaComponent, FName SocketName, FVector LocationOffset, FRotator RotationOffset)
{
   USkeletalMeshComponent* CharacterMesh = GetMesh();

   if(!IsValid(InKatanaComponent) || InKatanaComponent->GetOwner() != this
    || !IsValid(CharacterMesh) || !CharacterMesh->DoesSocketExist(SocketName))
    {
        return false;
    }

    const bool bAttached = InKatanaComponent->AttachToComponent(CharacterMesh,
    FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);

    if(!bAttached)
    {
        return false;
    }

    InKatanaComponent->SetRelativeLocationAndRotation(LocationOffset, RotationOffset);
    return true;


}
