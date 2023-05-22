// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/Notify/Equip/ALSAnimNotify_EquipState.h"

void UALSAnimNotify_EquipState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) {
	Super::Notify(MeshComp, Animation, EventReference);

	AALSCharacter* CharacterOwner = Cast<AALSCharacter>(MeshComp->GetOwner());
	if (!CharacterOwner) {
		return;
	}

	if (bEquipState == true) {
		IIEquipableObject_Basic* EquipableObjectInterface = Cast<IIEquipableObject_Basic>(CharacterOwner->EquippedActorComponent->GetChildActor());
		FEquipableObjectData_Basic EquipablableObjectData;
		EquipableObjectInterface->Execute_GetEquipableObjectData_Basic(CharacterOwner->EquippedActorComponent->GetChildActor(), EquipablableObjectData);
		CharacterOwner->SetOverlayState(EquipablableObjectData.State);
	}
	else {
		CharacterOwner->SetOverlayState(CharacterOwner->DefaultState);
	}
}