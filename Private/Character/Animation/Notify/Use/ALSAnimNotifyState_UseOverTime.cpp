// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/Notify/Use/ALSAnimNotifyState_UseOverTime.h"

void UALSAnimNotifyState_UseOverTime::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) {
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);


	if (!MeshComp) {
		return;
	}

	CharacterOwner = Cast<AALSCharacter>(MeshComp->GetOwner());
	if (!CharacterOwner) {
		return;
	}

	if (CharacterOwner) {
		CharacterOwner->EquippableObjectBasic_Interface->Execute_Use(CharacterOwner->EquippedActorComponent->GetChildActor());
	}
}

void UALSAnimNotifyState_UseOverTime::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) {
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!MeshComp) {
		return;
	}
}
