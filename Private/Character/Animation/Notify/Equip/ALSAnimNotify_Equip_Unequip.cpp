// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/Notify/Equip/ALSAnimNotify_Equip_Unequip.h"


void UALSAnimNotify_Equip_Unequip::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) {
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp) {
		return;
	}

	IIEquipSystem* EquipSystem = Cast<IIEquipSystem>(MeshComp->GetOwner());
	if (!EquipSystem) {
		return;
	}

	if (bEquip == true) {
		EquipSystem->Execute_Equip(MeshComp->GetOwner());
	}
	else {
		EquipSystem->Execute_Unequip(MeshComp->GetOwner());
	}
}