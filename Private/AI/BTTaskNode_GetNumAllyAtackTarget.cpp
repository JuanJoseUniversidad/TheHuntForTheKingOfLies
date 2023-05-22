// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTaskNode_GetNumAllyAtackTarget.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTaskNode_GetNumAllyAtackTarget::UBTTaskNode_GetNumAllyAtackTarget() {
	NodeName = "Get all the allies attacking same target";

	AffiliationTagName = "Affiliation.Hostile";
}

EBTNodeResult::Type UBTTaskNode_GetNumAllyAtackTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	
	if (!OwnerComp.GetAIOwner()) {
		return EBTNodeResult::Failed;
	}
	NumberOfAlliesNearestToTarget = 0;
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	TArray<AActor*> ActorsSensed;
	Cast<UAIPerceptionComponent>(OwnerComp.GetAIOwner()->GetComponentByClass(UAIPerceptionComponent::StaticClass()))->GetCurrentlyPerceivedActors(Sense, ActorsSensed);
	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName));
	
	float MaxNearesDistance = -1;
	if (TargetActor) {
		MaxNearesDistance = FVector::Distance(Pawn->GetActorLocation(), TargetActor->GetActorLocation());
	}

	for (AActor* Actor : ActorsSensed) {
		if (Actor != Pawn && Actor->ActorHasTag(AffiliationTagName)) {
			//Gets target which the ally has
			AActor* AllyTarget = Cast<AActor>(UAIBlueprintHelperLibrary::GetBlackboard(Actor)->GetValueAsObject(TargetKey.SelectedKeyName));
			
			//If an ally has the same target and its distance is less than self, then count it to use it on a BT and not attack the player all at the same time
			if (TargetActor && AllyTarget && AllyTarget == TargetActor) {
				float AllyDistance = FVector::Distance(Actor->GetActorLocation(), TargetActor->GetActorLocation());

				if (MaxNearesDistance != -1 && AllyDistance < MaxNearesDistance) {
					NumberOfAlliesNearestToTarget += 1;
				}
			}
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsInt(NumberOfAlliesWithSameTargetNearestKey.SelectedKeyName, NumberOfAlliesNearestToTarget);
	
	return EBTNodeResult::Succeeded;
}
