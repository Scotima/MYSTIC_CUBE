#include "DemonKing/FoxGirl/AN_AttachWeaponToSocket.h"
#include "DemonKing/FoxGirl/CFoxGirl.h"
#include "Components/ChildActorComponent.h"

void UAN_AttachWeaponToSocket::Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation, const FAnimNotifyEventReference &EventReference)
{
    Super::Notify(MeshComp, Animation, EventReference);
    if(!IsValid(MeshComp))
    {
        return;
    }
    ACFoxGirl* OwnerCharacter = Cast<ACFoxGirl>(MeshComp->GetOwner());

    if(!IsValid(OwnerCharacter))
    {
        return;
    }

    UChildActorComponent* KatanaComponent = 
        OwnerCharacter->FindComponentByClass<UChildActorComponent>();

    if(!IsValid(KatanaComponent))
    {
        return;
    }

    OwnerCharacter->AttachKatanaToSocket(
        KatanaComponent,
        AttachSettings.SocketName,
        AttachSettings.LocationOffset,
        AttachSettings.RotationOffset
    );

}