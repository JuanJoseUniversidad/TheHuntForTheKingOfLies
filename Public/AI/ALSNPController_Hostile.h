// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/ALSAIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "../Interfaces/IDashComponent.h"
#include "../Components/Dash_Component.h"
#include "../GeneralStructLibrary.h"
#include "ALSNPController_Hostile.generated.h"

class UAIPerceptionComponent;
class UBlackboardComponent;

/**
 * 
 */
UCLASS()
class ALSV4_CPP_API AALSNPController_Hostile : public AALSAIController
{
	GENERATED_BODY()

public:
	AALSNPController_Hostile();

	UFUNCTION()
		void OnPerception(AActor* Actor, FAIStimulus Stimulus);

protected:
	virtual void OnPossess(APawn* InPawn) override;

protected:
	UPROPERTY(VisibleAnywhere)
		UAIPerceptionComponent* AIPerceptionComp;

	UPROPERTY(VisibleAnywhere, Category = "Blackboard keys")
		FName TargetKeyName;

	UPROPERTY(VisibleAnywhere, Category = "Blackboard keys")
		FName PlayerTagKeyName;

	UPROPERTY(VisibleAnywhere, Category = "Blackboard keys")
		FName NumberOfAlliesWithSameTargetNearestKeyName;

	UPROPERTY(VisibleAnywhere, Category = "Blackboard keys")
		FName AffiliationTagName;

	UPROPERTY(VisibleAnywhere, Category = "Blackboard keys")
		FName MinDistanceToDashKeyName;

	UPROPERTY(EditAnywhere, Category = "Blackboard keys")
		float MinDistanceToDash;

	UPROPERTY(VisibleAnywhere, Category = "Blackboard keys")
		FName DashCooldownTagName;

	UPROPERTY(EditAnywhere, Category = "Blackboard keys", meta = (Units = "s"))
		float DashCooldown;

private:
	UBlackboardComponent* BB;

	int32 NumberOfAlliesNearestToTarget;

	TArray<AActor*> PercieveActors;
};
