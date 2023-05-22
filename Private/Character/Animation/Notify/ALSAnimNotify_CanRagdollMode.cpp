// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/Notify/ALSAnimNotify_CanRagdollMode.h"


UALSAnimNotify_CanRagdollMode::UALSAnimNotify_CanRagdollMode() {
	bCanRagdollMode = true;
}

void UALSAnimNotify_CanRagdollMode::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) {
	Super::Notify(MeshComp, Animation, EventReference);

	AALSBaseCharacter* Character = Cast<AALSBaseCharacter>(MeshComp->GetOwner());

	if (Character) {
		Character->SetCanRagdollMode(bCanRagdollMode);
	}

}
