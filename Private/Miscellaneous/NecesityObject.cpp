// Fill out your copyright notice in the Description page of Project Settings.


#include "Miscellaneous/NecesityObject.h"
#include "Engine/TargetPoint.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/Character.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"


UNecesityObject::UNecesityObject() {
	MinQuantityAllowed = -20.0f;
	MaxQuantityAllowed = 100.0f;
	AcceptanceRadiusToMove = 100.f;
	BlendOutStop = 0.2f;
	CannotFocusTag = "Status.CannotFocus";
	CannotFocusOtherLocation = true;
}

void UNecesityObject::StartCalculateNewNecesities() {
	if (MontageActions.Num() > 0) {
		int32 RandIndex = FMath::RandRange(0, MontageActions.Num()-1);
		MontageAction = MontageActions[RandIndex];

		if (Owner && MontageAction) {
			Cast<ACharacter>(Owner->GetPawn())->GetMesh()->GetAnimInstance()->Montage_Play(MontageAction);
		}
	}

	AALSBaseCharacter* ALSChar = Cast<AALSBaseCharacter>(Owner->GetPawn());
	if (ALSChar) {
		ALSChar->SetDesiredGait(EALSGait::Walking);
	}	
	
	//Focus a specific point
	if (LocationToFocus != FVector::ZeroVector) {
		UAIBlueprintHelperLibrary::GetAIController(ALSChar)->SetFocalPoint(LocationToFocus);
	}

	//Cannot be interrupted(for exaple the player starts to talk while its smithing something)
	if (CannotFocusOtherLocation) {
		Owner->GetPawn()->Tags.Add(CannotFocusTag);
	}

	Owner->GetWorldTimerManager().SetTimer(TimerHandleCalculateNewNecesities, this, &UNecesityObject::CalculateNewNecesities, TimeRegeneration, true);
}

void UNecesityObject::CalculateNewNecesities() {
	float NewNecesityQuantity = Owner->GetBlackboardComponent()->GetValueAsFloat(NecesityName);
	NewNecesityQuantity -= FMath::RandRange(MinQuantityPerRegeneration, MaxQuantityPerRegeneration);

	if (NewNecesityQuantity < MinQuantityAllowed) {
		NewNecesityQuantity = MinQuantityAllowed;
	}

	Owner->GetBlackboardComponent()->SetValueAsFloat(NecesityName, NewNecesityQuantity);

	PayOtherNecesities();

}

void UNecesityObject::OnMoveCompleted() {
	StartCalculateNewNecesities();
	Owner->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
	Owner->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(Owner);
}

void UNecesityObject::PayOtherNecesities() {
	for (auto& NecesityToPay : NecesitiesToPay) {
		float NewQuantity = Owner->GetBlackboardComponent()->GetValueAsFloat(NecesityToPay.BlackBoardKey) + NecesityToPay.NecesityCost;

		if (NewQuantity > MaxQuantityAllowed) {
			NewQuantity = MaxQuantityAllowed;
		}
		Owner->GetBlackboardComponent()->SetValueAsFloat(NecesityToPay.BlackBoardKey, NewQuantity);
	}
}

FName UNecesityObject::GetJobName() const {
	return JobName;
}

FName UNecesityObject::GetNecesityName() const {
	return NecesityName;
}

float UNecesityObject::GetMinQuantityPerRegeneration() const {
	return MinQuantityPerRegeneration;
}

float UNecesityObject::GetMaxQuantityPerRegeneration() const {
	return MaxQuantityPerRegeneration;
}

float UNecesityObject::GetMinTimeOnAction() const {
	return MinTimeOnAction;
}

float UNecesityObject::GetTimeRegeneration() const {
	return TimeRegeneration;
}

TArray<FVector> UNecesityObject::GetNecesityLocations() const {
	return LocationsReference;
}

UAnimMontage* UNecesityObject::GetMontageAction() const {
	return MontageAction;
}

TArray<FNecesityToPay> UNecesityObject::GetNecesitiesToPay() const {
	return NecesitiesToPay;
}

void UNecesityObject::Initialize(AAIController* NewController) {
	Owner = NewController;
}

void UNecesityObject::StartSatisfyNecesity() {
	Owner->GetPathFollowingComponent()->OnRequestFinished.AddUFunction(this, "OnMoveCompleted");
	Owner->MoveToLocation(GetClosestPoint(Owner->GetPawn()->GetActorLocation(), LocationsReference), AcceptanceRadiusToMove);
}

void UNecesityObject::StopSatisfyNecesity() {
	Owner->StopMovement();
	Owner->GetWorldTimerManager().ClearTimer(TimerHandleCalculateNewNecesities);
	Owner->GetPawn()->Tags.Remove(CannotFocusTag);
	if (Owner) {
		Cast<ACharacter>(Owner->GetPawn())->GetMesh()->GetAnimInstance()->StopAllMontages(BlendOutStop);
	}	
}

FVector UNecesityObject::GetClosestPoint(const FVector Point, const TArray<FVector>& PossiblesLocations) const {
	if (PossiblesLocations.Num() > 0) {
		float ClosestDistance = FVector::Distance(Point, PossiblesLocations[0]);
		float TemporalDistance = 0.0f;
		FVector ClosestLocation = PossiblesLocations[0];

		for (int i = 1; i < PossiblesLocations.Num(); i++) {
			TemporalDistance = FVector::Distance(Point, PossiblesLocations[i]);
			if (TemporalDistance < ClosestDistance) {
				ClosestDistance = TemporalDistance;
				ClosestLocation = PossiblesLocations[i];
			}
		}

		return ClosestLocation;
	}
	else {
		return Point;
	}
}
