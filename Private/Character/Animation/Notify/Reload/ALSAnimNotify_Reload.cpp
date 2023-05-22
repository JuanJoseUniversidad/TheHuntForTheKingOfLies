// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/Notify/Reload/ALSAnimNotify_Reload.h"

void UALSAnimNotify_Reload::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) {
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp) {
		return;
	}

	AALSCharacter* CharacterOwner = Cast<AALSCharacter>(MeshComp->GetOwner());
	if (!CharacterOwner) {
		return;
	}

	//First deletes from inventory
	CharacterOwner->InventoryComponent->DeleteObjectInventory_Implementation(CharacterOwner->EquippableObjectDataReload.CandidateLoad);
	//After reloads it
	CharacterOwner->EquippableObjectReload_Interface->Execute_Reload(CharacterOwner->EquippedActorComponent->GetChildActor());
}