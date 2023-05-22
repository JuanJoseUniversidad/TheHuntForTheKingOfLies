// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/Notify/Pick/ALSAnimNotify_Pick.h"

void UALSAnimNotify_Pick::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) {
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp) {
		return;
	}

	AALSCharacter* CharacterOwner = Cast<AALSCharacter>(MeshComp->GetOwner());
	if (!CharacterOwner) {
		return;
	}

	//Call interface
	CharacterOwner->PickComponent->PickableObjecBasic_Interface->Execute_Pick(CharacterOwner->PickComponent->PickComponentData.ObjectToPick, CharacterOwner);
	CharacterOwner->PickComponent->PickComponentData.PickableCandidates.Remove(CharacterOwner->PickComponent->PickComponentData.ObjectToPick);
	CharacterOwner->PickComponent->PickComponentData.ObjectToPick = nullptr;

	//Updates the HUD action notification with the next object to pick, nullptr means empty
	if (CharacterOwner->PickComponent) {
		if (!CharacterOwner->PickComponent->PickComponentData.PickableCandidates.IsEmpty()) {
			CharacterOwner->PickComponent->UpdateHUDBP(CharacterOwner->PickComponent->PickComponentData.PickableCandidates[0]);
		}
		else {
			CharacterOwner->PickComponent->UpdateHUDBP(nullptr);
		}
	}
}