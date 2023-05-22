// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "../../ALSBaseCharacter.h"
#include "ALSAnimNotify_CanRagdollMode.generated.h"

/**
 * 
 */
UCLASS()
class ALSV4_CPP_API UALSAnimNotify_CanRagdollMode : public UAnimNotify
{
	GENERATED_BODY()

	UALSAnimNotify_CanRagdollMode();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere, Category = "Properties")
		bool bCanRagdollMode;
};
