// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableActor.h"

// Sets default values
APickableActor::APickableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(RootComponent);
}


bool APickableActor::Pick_Implementation(AActor* Actor) {
	if (bCanBePicked == true) {
		bCanBePicked = false;

		//Execute sound
		if (PickableObjectData_Basic.PickSoundCue) {
			ACharacter* Character = Cast<ACharacter>(Actor);
			if (PickableObjectData_Basic.bAttachedSound == true) {
				UGameplayStatics::SpawnSoundAttached(PickableObjectData_Basic.PickSoundCue, Character->GetMesh());
			}
			else {
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickableObjectData_Basic.PickSoundCue, Character->GetActorLocation());
			}
		}

		//Inserts on the inventory
		if (!Actor->GetComponentsByInterface(UIInventoryComponent::StaticClass()).IsEmpty()) {
			UActorComponent* InventoryComponent = Actor->GetComponentsByInterface(UIInventoryComponent::StaticClass())[0];
			IIInventoryComponent* InventoryComponent_Interface = Cast<UInventory_Component>(InventoryComponent);
			InventoryComponent_Interface->Execute_InsertObjectInventory(InventoryComponent, PickableObjectData_Basic.ObjectClass, PickableObjectData_Basic.ObjectData);
		}

		//Destroys or regenerate after a time the picked object
		if (PickableObjectData_Basic.bDestroyOnPick == true) {
			Destroy();
		}
		else {
			MeshComponent->SetVisibility(false);
			MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetWorld()->GetTimerManager().SetTimer(TimerHandleRegenerate, [&]() {
				MeshComponent->SetVisibility(true);
				MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				bCanBePicked = true;
				GetWorld()->GetTimerManager().ClearTimer(TimerHandleRegenerate);
				}, PickableObjectData_Basic.TimeToRegenerate, false);
		}

		return true;
	}

	return false;
}

void APickableActor::GetPickableObjectData_Basic_Implementation(FPickableObjectData_Basic& PickableObjectStruct) {
	PickableObjectStruct = PickableObjectData_Basic;
}

void APickableActor::SetPickableObjectData_Basic_Implementation(FPickableObjectData_Basic PickableObjectStruct) {
	PickableObjectData_Basic = PickableObjectStruct;
}

// Called when the game starts or when spawned
void APickableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

