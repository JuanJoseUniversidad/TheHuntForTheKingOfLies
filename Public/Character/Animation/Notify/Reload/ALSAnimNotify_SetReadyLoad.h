// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "../../../ALSCharacter.h"
#include "Animation/AnimMontage.h"
#include "ALSAnimNotify_SetReadyLoad.generated.h"

/**
 * 
 */
UCLASS()
class ALSV4_CPP_API UALSAnimNotify_SetReadyLoad : public UAnimNotify
{
	GENERATED_BODY()
		
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

};
