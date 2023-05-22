// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "../../ALSCharacter.h"
#include "ALSAnimNotifyChangeOverlayState.generated.h"

/**
 * 
 */
UCLASS()
class ALSV4_CPP_API UALSAnimNotifyChangeOverlayState : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
		EALSOverlayState overlayState = EALSOverlayState::Default;
};
