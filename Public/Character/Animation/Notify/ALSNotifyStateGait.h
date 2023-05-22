// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "../../ALSCharacter.h"
#include "ALSNotifyStateGait.generated.h"

/**
 * 
 */
UCLASS()
class ALSV4_CPP_API UALSNotifyStateGait : public UAnimNotifyState
{
	GENERATED_BODY()

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	virtual FString GetNotifyName_Implementation() const override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimNotify)
		bool bModifyCurrentGait = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimNotify)
		EALSGait gait = EALSGait::Walking;
};
