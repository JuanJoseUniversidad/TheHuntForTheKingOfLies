// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/Notify/Use/ALSAnimNotify_InUse.h"

void UALSAnimNotify_InUse::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) {
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp) {
		return;
	}

	AALSCharacter* CharacterOwner = Cast<AALSCharacter>(MeshComp->GetOwner());
	if (!CharacterOwner) {
		return;
	}

	//Can use the object equipped again
	CharacterOwner->UseSystemData.bInUse = bInUse;
}