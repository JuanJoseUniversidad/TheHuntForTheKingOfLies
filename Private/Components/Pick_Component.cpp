// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Pick_Component.h"

// Sets default values for this component's properties
UPick_Component::UPick_Component()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	Owner = Cast<ACharacter>(GetOwner());

	//Set the channel to collide with only pickable objects!!
	PickTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Pick Trigger"));
	//Sets the trigger
	PickTrigger->OnComponentBeginOverlap.AddDynamic(this, &UPick_Component::OnOverlapBegin);
	PickTrigger->OnComponentEndOverlap.AddDynamic(this, &UPick_Component::OnOverlapEnd);
}


// Called when the game starts
void UPick_Component::BeginPlay()
{
	Super::BeginPlay();

	// ...
	PickTrigger->AttachToComponent(Owner->GetMesh(),FAttachmentTransformRules::KeepRelativeTransform);
}


// Called every frame
void UPick_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPick_Component::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	//Array of actors to ignore
	TArray<AActor*> ActorsToIgnore;
	FHitResult CamOutHit;
	ActorsToIgnore.Add(OtherActor);

	if (Cast<ACharacter>(GetOwner())->GetController()->LineOfSightTo(OtherActor) || !UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Owner->GetActorLocation(), (OtherActor->GetActorLocation() + FVector(0.0f, 0.0f, 20.0f)), 0.3f, ETraceTypeQuery::TraceTypeQuery1, true, ActorsToIgnore, EDrawDebugTrace::ForDuration, CamOutHit, true)) {
		//Only updates the HUD one time if the candidates queue is empty for initialice the HUD notification
		if (PickComponentData.PickableCandidates.IsEmpty()) {
			UpdateHUDBP(OtherActor);
		}

		PickComponentData.PickableCandidates.Push(OtherActor);
	}
}

void UPick_Component::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	PickComponentData.PickableCandidates.Remove(OtherActor);

	//If there is no more candidates to pick hide the notify
	if (PickComponentData.PickableCandidates.IsEmpty() && PickComponentData.bInPick == false) {
		UpdateHUDBP(nullptr);
	}
}

void UPick_Component::UpdateHUDBP_Implementation(AActor* Actor) {
}

void UPick_Component::Pick_Implementation() {
	if (!PickComponentData.PickableCandidates.IsEmpty()) {
		//Gets a candidate to pick
		PickComponentData.ObjectToPick = PickComponentData.PickableCandidates[0];
		PickComponentData.PickableCandidates.RemoveAt(0);

		//Gets the pickable candidate interface
		PickableObjecBasic_Interface = Cast<IIPickableObject_Basic>(PickComponentData.ObjectToPick);
		PickableObjecBasic_Interface->Execute_GetPickableObjectData_Basic(PickComponentData.ObjectToPick, PickableObjectData_Basic);


		Owner->GetMesh()->GetAnimInstance()->Montage_Play(PickableObjectData_Basic.PickAnimation, 1.0f);

		//Calls pick interface function in a event in the montage
	}
}

void UPick_Component::GetPickComponentData_Implementation(FPickComponentData& PickComponentStruct) {
	PickComponentStruct = PickComponentData;
}

void UPick_Component::SetPickComponentData_Implementation(FPickComponentData PickComponentStruct) {
	PickComponentData = PickComponentStruct;
}

