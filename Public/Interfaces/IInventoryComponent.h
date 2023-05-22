// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../GeneralStructLibrary.h"
#include "IInventoryComponent.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIInventoryComponent : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ALSV4_CPP_API IIInventoryComponent
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/**
	 * Inserts a new object on the inventory
	 * @param Subclass - The subclass to find on the inventory
	 * @param Data - Struct of data for the object to insert
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IInventoryComponent functions")
		void InsertObjectInventory(TSubclassOf<AActor> Subclass, FInventoryObjectData Data);

	/**
	 * Deletes a object of the inventory
	 * @param Subclass - The subclass to find on the inventory
	 * @param Quantity - The quantity to delete of the object
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IInventoryComponent functions")
		void DeleteObjectInventory(TSubclassOf<AActor> Subclass, int32 Quantity = 1);

	/**
	 * Gets a reference of the data structure of the dash component data
	 * @param DashComponentStruct - Reference to the variable to be assignated the data struct
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IInventoryComponent functions")
		void GetInventoryComponentData(FInventoryComponentData& InventoryComponentStruct);

	/**
	 * Sets a new data structure of the dash component data
	 * @param DashComponentStruct - New data struct to assing
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IInventoryComponent functions")
		void SetInventoryComponentData(FInventoryComponentData InventoryComponentStruct);
};
