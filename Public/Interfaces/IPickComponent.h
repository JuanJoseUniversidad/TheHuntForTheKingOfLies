// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../GeneralStructLibrary.h"
#include "IPickComponent.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIPickComponent : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ALSV4_CPP_API IIPickComponent
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * Picks the first object on the queue of candidates
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IPickComponent functions")
		void Pick();

	/**
	 * Gets a reference of the data structure of the pick component data
	 * @param PickComponentStruct - Reference to the variable to be assignated the data struct
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IPickComponent functions")
		void GetPickComponentData(FPickComponentData& PickComponentStruct);

	/**
	 * Sets a new data structure of the pick component data
	 * @param PickComponentStruct - New data struct to assing
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IPickComponent functions")
		void SetPickComponentData(FPickComponentData PickComponentStruct);
};
