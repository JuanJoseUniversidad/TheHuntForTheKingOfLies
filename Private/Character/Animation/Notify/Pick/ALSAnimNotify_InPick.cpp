// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/Notify/Pick/ALSAnimNotify_InPick.h"

void UALSAnimNotify_InPick::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) {
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp) {
		return;
	}

	AALSCharacter* CharacterOwner = Cast<AALSCharacter>(MeshComp->GetOwner());
	if (!CharacterOwner) {
		return;
	}

	CharacterOwner->PickComponent->PickComponentData.bInPick = bInPick;
}