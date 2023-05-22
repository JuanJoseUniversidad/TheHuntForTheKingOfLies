// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Interfaces/IInventoryComponent.h"
#include "../GeneralStructLibrary.h"
#include "Inventory_Component.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALSV4_CPP_API UInventory_Component : public UActorComponent, public IIInventoryComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventory_Component();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* Functions */
	virtual void InsertObjectInventory_Implementation(TSubclassOf<AActor> Subclass, FInventoryObjectData Data) override;
	virtual void DeleteObjectInventory_Implementation(TSubclassOf<AActor> Subclass, int32 Quantity = 1) override;
	virtual void GetInventoryComponentData_Implementation(FInventoryComponentData& InventoryComponentStruct) override;
	virtual void SetInventoryComponentData_Implementation(FInventoryComponentData InventoryComponentStruct) override;

public:
	/* Variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory | Properties")
		FInventoryComponentData InventoryComponentData;
};
