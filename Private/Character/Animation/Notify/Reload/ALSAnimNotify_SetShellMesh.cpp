// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/Notify/Reload/ALSAnimNotify_SetShellMesh.h"

void UALSAnimNotify_SetShellMesh::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) {
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
		FProjectileData ProjectileData;

		AActor* Actor = CharacterOwner->GetWorld()->SpawnActor(CharacterOwner->EquippableObjectDataReload.CandidateLoad);
		Cast<IIProjectile_Basic>(Actor)->Execute_GetProjectileData(Actor, ProjectileData);
		Mesh = ProjectileData.Mesh;
		Actor->Destroy();
	}

	CharacterOwner->ReloadShellAnchor->SetStaticMesh(Mesh);
}