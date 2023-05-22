// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/Notify/Use/ALSAnimNotify_Use.h"

void UALSAnimNotify_Use::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) {
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp) {
		return;
	}

	AALSCharacter* CharacterOwner = Cast<AALSCharacter>(MeshComp->GetOwner());
	if (!CharacterOwner) {
		return;
	}

	/*Use the equiped object*/
	CharacterOwner->EquippableObjectBasic_Interface->Execute_Use(CharacterOwner->EquippedActorComponent->GetChildActor());
}