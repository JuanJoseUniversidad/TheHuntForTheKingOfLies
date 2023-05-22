// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory_Component.h"

// Sets default values for this component's properties
UInventory_Component::UInventory_Component()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UInventory_Component::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventory_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventory_Component::InsertObjectInventory_Implementation(TSubclassOf<AActor> Subclass, FInventoryObjectData Data) {
	if (InventoryComponentData.Inventory.Contains(Subclass) == false) {
		InventoryComponentData.Inventory.Add(Subclass, Data);
	}
	else {
		InventoryComponentData.Inventory.Find(Subclass)->Quantity += Data.Quantity;
	}
}

void UInventory_Component::DeleteObjectInventory_Implementation(TSubclassOf<AActor> Subclass, int32 Quantity) {
	if (InventoryComponentData.Inventory.Find(Subclass)) {
		if (InventoryComponentData.Inventory.Find(Subclass)->Quantity <= 1) {
			InventoryComponentData.Inventory.Remove(Subclass);
		}
		else {
			InventoryComponentData.Inventory.Find(Subclass)->Quantity -= Quantity;
		}
	}
}

void UInventory_Component::GetInventoryComponentData_Implementation(FInventoryComponentData& InventoryComponentStruct) {
	InventoryComponentStruct = InventoryComponentData;
}

void UInventory_Component::SetInventoryComponentData_Implementation(FInventoryComponentData InventoryComponentStruct) {
	InventoryComponentData = InventoryComponentStruct;
}

