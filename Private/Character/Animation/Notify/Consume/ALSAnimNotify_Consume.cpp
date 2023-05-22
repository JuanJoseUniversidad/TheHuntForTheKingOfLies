// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/Notify/Consume/ALSAnimNotify_Consume.h"

void UALSAnimNotify_Consume::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) {
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp) {
		return;
	}

	AALSCharacter* CharacterOwner = Cast<AALSCharacter>(MeshComp->GetOwner());
	if (!CharacterOwner) {
		return;
	}

	//Call interface
	//Consume
	IIConsumableObject_Basic* ConsumableObjectBasic_Interface = Cast<IIConsumableObject_Basic>(CharacterOwner->ConsumeComponent->ConsumeComponentData.ActorConsumable);
	ConsumableObjectBasic_Interface->Execute_Consume(CharacterOwner->ConsumeComponent->ConsumeComponentData.ActorConsumable, CharacterOwner);

	//Deletes from inventory
	//CharacterOwner->InventoryComponent->DeleteObjectInventory_Implementation(CharacterOwner->ConsumeComponent->ConsumeComponentData.ConsumableClass);
	//Inserts on the inventory
	if (!CharacterOwner->GetComponentsByInterface(UIInventoryComponent::StaticClass()).IsEmpty()) {
		UActorComponent* InventoryComponent = CharacterOwner->GetComponentsByInterface(UIInventoryComponent::StaticClass())[0];
		IIInventoryComponent* InventoryComponent_Interface = Cast<UInventory_Component>(InventoryComponent);
		InventoryComponent_Interface->Execute_DeleteObjectInventory(InventoryComponent, CharacterOwner->ConsumeComponent->ConsumeComponentData.ConsumableClass, 1);
	}

	//Delete the object created
	CharacterOwner->ConsumeComponent->ConsumeComponentData.ActorConsumable->Destroy();
}