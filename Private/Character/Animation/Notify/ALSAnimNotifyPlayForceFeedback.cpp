// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/Notify/ALSAnimNotifyPlayForceFeedback.h"

void UALSAnimNotifyPlayForceFeedback::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (UGameplayStatics::GetPlayerController(GetWorld(), 0)) {
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->ClientPlayForceFeedback(feedbackForce);
	}
}