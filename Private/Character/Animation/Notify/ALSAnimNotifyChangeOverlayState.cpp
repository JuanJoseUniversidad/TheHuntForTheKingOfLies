// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/Notify/ALSAnimNotifyChangeOverlayState.h"

void UALSAnimNotifyChangeOverlayState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp)
	{
		return;
	}

	AALSCharacter* characterOwner = Cast<AALSCharacter>(MeshComp->GetOwner());
	if (!characterOwner)
	{
		return;
	}

	characterOwner->SetOverlayState(overlayState);
}