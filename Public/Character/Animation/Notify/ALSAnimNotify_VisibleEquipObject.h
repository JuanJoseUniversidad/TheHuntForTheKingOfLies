// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "../../ALSCharacter.h"
#include "ALSAnimNotify_VisibleEquipObject.generated.h"

/**
 * 
 */
UCLASS()
class ALSV4_CPP_API UALSAnimNotify_VisibleEquipObject : public UAnimNotify
{
	GENERATED_BODY()
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere, Category = "Properties")
		bool bIsVisible;
};
