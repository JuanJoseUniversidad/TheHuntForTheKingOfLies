// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/Notify/Reload/ALSAnimNotify_SetReadyLoad.h"

void UALSAnimNotify_SetReadyLoad::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) {
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp) {
		return;
	}

	AALSCharacter* CharacterOwner = Cast<AALSCharacter>(MeshComp->GetOwner());
	if (!CharacterOwner) {
		return;
	}

	//Set ready load
	CharacterOwner->EquippableObjectReload_Interface->Execute_SetReadyLoad(CharacterOwner->EquippedActorComponent->GetChildActor());
}