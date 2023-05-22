// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_GetAlliesAttackTarget.generated.h"

class UAISense;

/**
 * 
 */
UCLASS()
class ALSV4_CPP_API UBTService_GetAlliesAttackTarget : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_GetAlliesAttackTarget();

	UPROPERTY(Category = Navigation, EditAnywhere)
		TSubclassOf<UAISense> Sense;

	UPROPERTY(EditAnywhere, Category = "Blackboard keys")
		FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard keys")
		FBlackboardKeySelector NumberOfAlliesWithSameTargetNearestKey;

	UPROPERTY(VisibleAnywhere, Category = "Blackboard keys")
		FName AffiliationTagName;

private:
	int32 NumberOfAlliesNearestToTarget;

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
