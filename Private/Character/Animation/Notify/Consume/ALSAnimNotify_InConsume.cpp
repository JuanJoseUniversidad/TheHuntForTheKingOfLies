// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/Notify/Consume/ALSAnimNotify_InConsume.h"

void UALSAnimNotify_InConsume::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) {
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp) {
		return;
	}

	AALSCharacter* CharacterOwner = Cast<AALSCharacter>(MeshComp->GetOwner());
	if (!CharacterOwner) {
		return;
	}

	CharacterOwner->ConsumeComponent->ConsumeComponentData.bInConsume = bInConsume;
}