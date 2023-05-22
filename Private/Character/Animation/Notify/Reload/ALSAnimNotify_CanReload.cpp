// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/Notify/Reload/ALSAnimNotify_CanReload.h"

void UALSAnimNotify_CanReload::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) {
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp) {
		return;
	}

	AALSCharacter* CharacterOwner = Cast<AALSCharacter>(MeshComp->GetOwner());
	if (!CharacterOwner) {
		return;
	}

	CharacterOwner->ReloadSystemData.bCanReload = bCanReload;

	IIReloadSystem* ReloadInterface = Cast<IIReloadSystem>(CharacterOwner);
	if (ReloadInterface) {
		ReloadInterface->Execute_ReloadMode(CharacterOwner, CharacterOwner->bWantToReload);
	}
}