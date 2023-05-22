// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Consume_Component.h"

// Sets default values for this component's properties
UConsume_Component::UConsume_Component()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	Owner = Cast<ACharacter>(GetOwner());

	ConsumableAnchor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Consumable anchor"));
}


// Called when the game starts
void UConsume_Component::BeginPlay()
{
	Super::BeginPlay();

	// ...


	ConsumableAnchor->AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ConsumeComponentData.SocketNameConsumable);

}


// Called every frame
void UConsume_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UConsume_Component::StartConsuming_Implementation() {
	ConsumeComponentData.bInConsume = true;

	//Creates the object and gets the data
	ConsumeComponentData.ActorConsumable = Owner->GetWorld()->SpawnActor(ConsumeComponentData.ConsumableClass);
	ConsumableObjectBasic_Interface = Cast<IIConsumableObject_Basic>(ConsumeComponentData.ActorConsumable);
	ConsumableObjectBasic_Interface->Execute_GetConsumableObjectData_Basic(ConsumeComponentData.ActorConsumable, ConsumableObjectData_Basic);
	//Execute animation
	Owner->GetMesh()->GetAnimInstance()->Montage_Play(ConsumableObjectData_Basic.ConsumableAnimation, 1.0f);
}

