// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/HUD.h"
#include "GeneralHUD.generated.h"

/**
 * 
 */
UCLASS()
class PERSEGUIDORPROJECT_API AGeneralHUD : public AHUD
{
	GENERATED_BODY()
		//funcion para fade del puntero

public:
	AGeneralHUD();

	virtual void BeginPlay() override;
};
