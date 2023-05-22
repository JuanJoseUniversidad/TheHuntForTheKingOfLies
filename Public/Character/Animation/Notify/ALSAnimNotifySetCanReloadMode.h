// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "../../ALSCharacter.h"
#include "ALSAnimNotifySetCanReloadMode.generated.h"

/**
 * 
 */
UCLASS()
class ALSV4_CPP_API UALSAnimNotifySetCanReloadMode : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		bool bEnableReloadMode = true;
};
