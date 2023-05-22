// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/Notify/ALSAnimNotify_VisibleEquipObject.h"

void UALSAnimNotify_VisibleEquipObject::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) {
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp) {
		return;
	}

	AALSCharacter* CharacterOwner = Cast<AALSCharacter>(MeshComp->GetOwner());
	if (!CharacterOwner) {
		return;
	}

	/*Hide the equiped object*/
	CharacterOwner->SetEquippableObjectVisible(bIsVisible);
}