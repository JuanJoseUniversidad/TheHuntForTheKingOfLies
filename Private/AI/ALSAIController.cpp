// Copyright:       Copyright (C) 2022 Doğa Can Yanıkoğlu
// Source Code:     https://github.com/dyanikoglu/ALS-Community

#include "AI/ALSAIController.h"

#include "Character/ALSBaseCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

AALSAIController::AALSAIController()
{
	RagdollKeyName = "RagdollMode";
}

void AALSAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (Behaviour && InPawn)
	{
		RunBehaviorTree(Behaviour);
	}

	Cast<AALSBaseCharacter>(GetPawn())->RagdollStateChangedDelegate.AddDynamic(this, &AALSAIController::RagdollStateChange);
}

FVector AALSAIController::GetFocalPointOnActor(const AActor* Actor) const
{
	if (Actor == nullptr)
	{
		return FAISystem::InvalidLocation;
	}
	const APawn* FocusPawn = Cast<APawn>(Actor);
	if (FocusPawn)
	{
		// Focus on pawn's eye view point
		return FocusPawn->GetPawnViewLocation();
	}
	return Actor->GetActorLocation();
}

void AALSAIController::RagdollStateChange(bool bRagdollState) {
	GetBlackboardComponent()->SetValueAsBool(RagdollKeyName,bRagdollState);
}
