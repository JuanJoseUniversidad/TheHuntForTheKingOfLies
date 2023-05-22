// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "../../../ALSCharacter.h"
#include "../../../../Interfaces/IProjectile_Basic.h"
#include "../../../../GeneralStructLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "ALSAnimNotify_SetShellMesh.generated.h"

/**
 * 
 */
UCLASS()
class ALSV4_CPP_API UALSAnimNotify_SetShellMesh : public UAnimNotify
{
	GENERATED_BODY()
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		bool bInvisibleMesh = false;
};
