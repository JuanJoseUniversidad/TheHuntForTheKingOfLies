// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/ALSAIController.h"
#include "../Character/ALSNPCCharacter_NoHostile.h"
#include "Perception/AIPerceptionTypes.h"
#include "ALSNPController_NoHostile.generated.h"

class UAIPerceptionComponent;
class UBlackboardComponent;

/**
 * 
 */
UCLASS()
class ALSV4_CPP_API AALSNPController_NoHostile : public AALSAIController
{
	GENERATED_BODY()

public:
	AALSNPController_NoHostile();

	UFUNCTION()
		void OnPerception(AActor* Actor, FAIStimulus Stimulus);

protected:
	virtual void OnPossess(APawn* InPawn) override;

protected:
	UPROPERTY(VisibleAnywhere)
		UAIPerceptionComponent* AIPerceptionComp;

	UPROPERTY(VisibleAnywhere, Category = "Blackboard keys")
		FName HostileDetectedKeyName;

	UPROPERTY(VisibleAnywhere, Category = "Blackboard keys")
		FName AffiliationTagName;

private:
	UBlackboardComponent* BB;
};
