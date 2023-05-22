// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../GeneralStructLibrary.h"
#include "IInteractComponent.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIInteractComponent : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ALSV4_CPP_API IIInteractComponent
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * Interacts the first object which has entered on the interact range
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IInteractComponent functions")
		void Interact();

	/**
	 * Gets a reference of the data structure of the interact component data
	 * @param InteractComponentStruct - Reference to the variable to be assignated the data struct
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IInteractComponent functions")
		void GetInteractComponentData(FInteractComponentData& InteractComponentStruct);

	/**
	 * Sets a new data structure of the interact component data
	 * @param InteractComponentStruct - New data struct to assing
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IInteractComponent functions")
		void SetInteractComponentData(FInteractComponentData InteractComponentStruct);
};
