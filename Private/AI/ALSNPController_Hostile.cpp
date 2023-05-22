// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ALSNPController_Hostile.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"


#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/Character.h"

AALSNPController_Hostile::AALSNPController_Hostile() {
	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));
	TargetKeyName = "Target";
	PlayerTagKeyName = "Player";
	NumberOfAlliesWithSameTargetNearestKeyName = "NumberOfAlliesWithSameTargetNearest";
	AffiliationTagName = "Affiliation.Hostile";
	MinDistanceToDashKeyName = "MinDistanceToDash";
	DashCooldownTagName = "DashCooldown";

	MinDistanceToDash = 500.0f;
	DashCooldown = 5.0f;

	NumberOfAlliesNearestToTarget = 0;
}

void AALSNPController_Hostile::OnPerception(AActor* Actor, FAIStimulus Stimulus) {
	AActor* Target = nullptr;
	float MaxNearesDistance = -1.0f;
	float NewNewaresDistance = -1.0f;
	

	if (Stimulus.WasSuccessfullySensed()) {
		PercieveActors.Add(Actor);
	}
	else {
		PercieveActors.Remove(Actor);
	}

	for (AActor* Actor : PercieveActors) {
		if (Actor != GetPawn() && !Actor->ActorHasTag(AffiliationTagName)) {
			//If the player is percive among the other actors, then set it as target and exit the loop
			if (Actor->ActorHasTag(PlayerTagKeyName)) {
				Target = Actor;
				break;
			}

			//If the player is not percive then persecute other potential targets nearest to self
			NewNewaresDistance = FVector::Distance(GetPawn()->GetActorLocation(), Actor->GetActorLocation());
			if (NewNewaresDistance > MaxNearesDistance || NewNewaresDistance == -1) {
				MaxNearesDistance = NewNewaresDistance;
				Target = Actor;
			}
		}
		/*else if (Actor != GetPawn() && Actor->ActorHasTag(AffiliationTagName)) {
			//Gets target which the ally has
			AActor* AllyTarget = Cast<AActor>(UAIBlueprintHelperLibrary::GetBlackboard(Actor)->GetValueAsObject(TargetKeyName));
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Some debug message!"));
			//If an ally has the same target and its distance is less than self, then count it to use it on a BT and not attack the player all at the same time
			if (Target && AllyTarget && AllyTarget == Target) {
				if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("in"));
				float AllyDistance = FVector::Distance(Actor->GetActorLocation(), Target->GetActorLocation());

				if (AllyDistance < MaxNearesDistance) {
					if (GEngine)
						GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("inin"));
					NumberOfAlliesNearestToTarget += 1;
				}
			}
		}*/
	}
		

	if (ensure(BB)) {
		BB->SetValueAsObject(TargetKeyName, Target);
		BB->SetValueAsInt(NumberOfAlliesWithSameTargetNearestKeyName, NumberOfAlliesNearestToTarget);
	}
}

void AALSNPController_Hostile::OnPossess(APawn* InPawn) {
	Super::OnPossess(InPawn);
	BB = GetBlackboardComponent();

	//Tries to set the personalize dash values of each possed pawn
	FDashComponentData DashComponentData;
	UActorComponent* DashComponent = InPawn->GetComponentsByInterface(UIDashComponent::StaticClass())[0];
	IIDashComponent* DashInterface = Cast<IIDashComponent>(DashComponent);
	if (DashInterface) {
		DashInterface->Execute_GetDashComponentData(DashComponent, DashComponentData);

		BB->SetValueAsFloat(MinDistanceToDashKeyName, DashComponentData.DistanceToDashAI);
		BB->SetValueAsFloat(DashCooldownTagName, DashComponentData.CoolDownDashAI);
	}
	else {
		BB->SetValueAsFloat(MinDistanceToDashKeyName, 500.0f);
		BB->SetValueAsFloat(DashCooldownTagName, 5.0f);
	}


	
	AIPerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AALSNPController_Hostile::OnPerception);
}
