// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/Notify/Use/ALSAnimNotify_SetUseIfAiming.h"

void UALSAnimNotify_SetUseIfAiming::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) {
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp) {
		return;
	}

	AALSCharacter* CharacterOwner = Cast<AALSCharacter>(MeshComp->GetOwner());
	if (!CharacterOwner) {
		return;
	}

	/*If the player has nothing equipped and its aiming then enables bCanUse and play the aiming sound*/
	if (CharacterOwner && CharacterOwner->GetRotationMode() == EALSRotationMode::Aiming) {
		CharacterOwner->EquippableObjectBasic_Interface->Execute_PlayReadySound(CharacterOwner->EquippedActorComponent->GetChildActor());
		CharacterOwner->EnableCanUse(0.1f);
	}
}