// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_GetNumAllyAtackTarget.generated.h"

class UAISense;

/**
 * 
 */
UCLASS()
class ALSV4_CPP_API UBTTaskNode_GetNumAllyAtackTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTaskNode_GetNumAllyAtackTarget();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


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
};
