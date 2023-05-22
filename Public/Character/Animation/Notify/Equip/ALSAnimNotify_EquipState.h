// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "../../../../GeneralStructLibrary.h"
#include "../../../../Interfaces/IEquipableObject_Basic.h"
#include "ALSAnimNotify_EquipState.generated.h"

/**
 * 
 */
UCLASS()
class ALSV4_CPP_API UALSAnimNotify_EquipState : public UAnimNotify
{
	GENERATED_BODY()
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		bool bEquipState = true;
};
