// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/Notify/Consume/ALSAnimNotify_SetConsumeMesh.h"

void UALSAnimNotify_SetConsumeMesh::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) {
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp) {
		return;
	}

	AALSCharacter* CharacterOwner = Cast<AALSCharacter>(MeshComp->GetOwner());
	if (!CharacterOwner) {
		return;
	}

	UStaticMesh* Mesh = nullptr;
	if (bInvisibleMesh == false) {
		Mesh = CharacterOwner->ConsumeComponent->ConsumableObjectData_Basic.Mesh;
	}

	CharacterOwner->ConsumeComponent->ConsumableAnchor->SetStaticMesh(Mesh);
}