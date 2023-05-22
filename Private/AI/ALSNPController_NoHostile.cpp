// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ALSNPController_NoHostile.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"

AALSNPController_NoHostile::AALSNPController_NoHostile() {
	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));
	HostileDetectedKeyName = "ThreatDetected";
	AffiliationTagName = "Affiliation.Villager";
}

void AALSNPController_NoHostile::OnPerception(AActor* Actor, FAIStimulus Stimulus) {
	if (Actor != GetPawn() && !Actor->ActorHasTag(AffiliationTagName)) {
		if (Stimulus.WasSuccessfullySensed() && BB) {
			BB->SetValueAsObject(HostileDetectedKeyName, Actor);
		}
		else if (!Stimulus.WasSuccessfullySensed() && BB) {
			BB->SetValueAsObject(HostileDetectedKeyName, nullptr);
		}
	}
}

void AALSNPController_NoHostile::OnPossess(APawn* InPawn) {
	Super::OnPossess(InPawn);

	AALSNPCCharacter_NoHostile* NPC = Cast<AALSNPCCharacter_NoHostile>(InPawn);
	if (ensureMsgf(NPC, TEXT("The possed pawn must be an non hostile npc or derived"))) {
		NPC->ConstructNecesities();
		BB = GetBlackboardComponent();
		if (BB) {
			for (auto& NecesityObj : NPC->GetNecesityActions()) {
				BB->SetValueAsObject(NecesityObj.Key, NecesityObj.Value);
			}

			BB->SetValueAsVector(NPC->GetSleepLocationKeyName(), NPC->GetSleepLocation());
		}
	}

	AIPerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AALSNPController_NoHostile::OnPerception);
}
