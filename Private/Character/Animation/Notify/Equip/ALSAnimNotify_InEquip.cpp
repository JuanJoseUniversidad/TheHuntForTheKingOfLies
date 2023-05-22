// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/Notify/Equip/ALSAnimNotify_InEquip.h"


void UALSAnimNotify_InEquip::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) {
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp) {
		return;
	}

	IIEquipSystem* EquipSystem = Cast<IIEquipSystem>(MeshComp->GetOwner());
	if (!EquipSystem) {
		return;
	}

	FEquipSystemData EquipSystemData;
	EquipSystem->Execute_GetEquipSystemData(MeshComp->GetOwner(), EquipSystemData);
	EquipSystemData.bInEquip = bInEquip;
	EquipSystem->Execute_SetEquipSystemData(MeshComp->GetOwner(), EquipSystemData);
}