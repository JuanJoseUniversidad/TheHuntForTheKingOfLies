// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../GeneralStructLibrary.h"
#include "IPickableObject_Basic.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIPickableObject_Basic : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ALSV4_CPP_API IIPickableObject_Basic
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * Picks the item and adds it to an inventory
	 * @param Actor - Actor where to retrive the inventory
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IPickableObject_Basic functions")
		bool Pick(AActor* Actor);

	/**
	 * Gets a reference of the data structure of the pickable object data
	 * @param ReloadSystemStruct - Reference to the variable to be assignated the data struct
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IPickableObject_Basic functions")
		void GetPickableObjectData_Basic(FPickableObjectData_Basic& PickableObjectStruct);

	/**
	 * Sets a new data structure of the pickable object data
	 * @param ReloadSystemStruct - New data struct to assing
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IPickableObject_Basic functions")
		void SetPickableObjectData_Basic(FPickableObjectData_Basic PickableObjectStruct);
};
