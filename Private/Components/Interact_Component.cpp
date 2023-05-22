// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Interact_Component.h"

// Sets default values for this component's properties
UInteract_Component::UInteract_Component()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	Owner = Cast<ACharacter>(GetOwner());

	InteractTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Interact trigger"));
	InteractTrigger->OnComponentBeginOverlap.AddDynamic(this, &UInteract_Component::OnOverlapBegin);
	InteractTrigger->OnComponentEndOverlap.AddDynamic(this, &UInteract_Component::OnOverlapEnd);

	CannotInteractTag = "Status.Busy";
}


// Called when the game starts
void UInteract_Component::BeginPlay()
{
	Super::BeginPlay();

	// ...
	InteractTrigger->AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
}


// Called every frame
void UInteract_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInteract_Component::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	//Array of actors to ignore
	TArray<AActor*> ActorsToIgnore;
	FHitResult CamOutHit;
	ActorsToIgnore.Add(OtherActor);

	if (!OtherActor->ActorHasTag(CannotInteractTag) && (Cast<ACharacter>(GetOwner())->GetController()->LineOfSightTo(OtherActor) || !UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Owner->GetActorLocation(),(OtherActor->GetActorLocation()+FVector(0.0f,0.0f,20.0f)), 0.3f, ETraceTypeQuery::TraceTypeQuery1, true, ActorsToIgnore, EDrawDebugTrace::ForDuration, CamOutHit, true))) {
		InteractComponentData.ObjectToInteract = OtherActor;
		UpdateHUDBP(OtherActor);
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Enter"));
	}
}

void UInteract_Component::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	InteractComponentData.ObjectToInteract = nullptr;
	UpdateHUDBP(nullptr);
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Exit"));
}

void UInteract_Component::UpdateHUDBP_Implementation(AActor* Actor) {
}

void UInteract_Component::Interact_Implementation() {
	if (InteractComponentData.ObjectToInteract) {
		ObjectWithInterface.Empty();

		//Try to cast the object for obtain the interface and adds it to the list
		if (Cast<IIInteractable_Basic>(InteractComponentData.ObjectToInteract)) {
			ObjectWithInterface.Add(InteractComponentData.ObjectToInteract);
		}
		
		//Finds if there is also any component which implements the interface and adds it to the list
		for (auto& Component : InteractComponentData.ObjectToInteract->GetComponentsByInterface(UIInteractable_Basic::StaticClass())) {
			ObjectWithInterface.Add(Component);
		}
		
		//Execute all the interactables interfaces found
		for (auto& Object : ObjectWithInterface) {
			Cast<IIInteractable_Basic>(Object)->Execute_Interact(Object, GetOwner());
		}
	}
}

void UInteract_Component::GetInteractComponentData_Implementation(FInteractComponentData& InteractComponentStruct) {
	InteractComponentStruct = InteractComponentData;
}

void UInteract_Component::SetInteractComponentData_Implementation(FInteractComponentData InteractComponentStruct) {
	InteractComponentData = InteractComponentStruct;
}

