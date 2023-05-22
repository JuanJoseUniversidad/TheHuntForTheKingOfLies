// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ALSNPCCharacter_NoHostile.h"
#include "AIController.h"

AALSNPCCharacter_NoHostile::AALSNPCCharacter_NoHostile(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	SleepLocationKeyName = "SleepLocation";
}

TMap<FName, UNecesityObject*> AALSNPCCharacter_NoHostile::GetNecesityActions() {
	return NecesityActions;
}

FVector AALSNPCCharacter_NoHostile::GetSleepLocation() {
	return SleepLocation;
}

FName AALSNPCCharacter_NoHostile::GetSleepLocationKeyName() {
	return SleepLocationKeyName;
}

void AALSNPCCharacter_NoHostile::ConstructNecesities() {
	//construct the necesities
	for (auto& Necesity : ActionsToSatisfyNecesitiesClasses) {
		UNecesityObject* Obj = NewObject<UNecesityObject>(this, Necesity);
		Obj->Initialize(Cast<AAIController>(GetController()));
		NecesityActions.Add(Obj->GetJobName(), Obj);
	}
}