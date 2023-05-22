// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/Notify/ALSNotifyStateGait.h"

void UALSNotifyStateGait::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference){

	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);


	if (!MeshComp){
		return;
	}

	AALSCharacter* characterOwner = Cast<AALSCharacter>(MeshComp->GetOwner());
	if (!characterOwner){
		return;
	}

	if (bModifyCurrentGait == true) {
		characterOwner->SetDesiredGait(gait);
	}
}

void UALSNotifyStateGait::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference){
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!MeshComp){
		return;
	}

	AALSCharacter* characterOwner = Cast<AALSCharacter>(MeshComp->GetOwner());
	if (!characterOwner){
		return;
	}

	characterOwner->ResetOriginalGait();
}

FString UALSNotifyStateGait::GetNotifyName_Implementation() const
{
	FString Name(TEXT("Gait"));
	return Name;
}