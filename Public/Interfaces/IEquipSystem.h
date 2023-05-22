// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../GeneralStructLibrary.h"
#include "IEquipSystem.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIEquipSystem : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ALSV4_CPP_API IIEquipSystem
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * Called when equip the object
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IEquipSystem functions")
		void Equip();

	/**
	 * Called when equip the object
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IEquipSystem functions")
		void Unequip();

	/**
	 * Gets a reference of the data structure of the equip system data
	 * @param EquipSystemStruct - Reference to the variable to be assignated the data struct
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IEquipSystem functions")
		void GetEquipSystemData(FEquipSystemData& EquipSystemStruct);

	/**
	 * Sets a new data structure of the equip system data
	 * @param EquipSystemStruct - New data struct to assing
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IEquipSystem functions")
		void SetEquipSystemData(FEquipSystemData EquipSystemStruct);
};
