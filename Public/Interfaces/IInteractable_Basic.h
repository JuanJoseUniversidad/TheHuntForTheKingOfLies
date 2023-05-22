// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../GeneralStructLibrary.h"
#include "IInteractable_Basic.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIInteractable_Basic : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ALSV4_CPP_API IIInteractable_Basic
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * Starts the interaction with the item
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IInteractable_Basic functions")
		bool Interact(AActor* Interactor = nullptr);

	/**
	 * Ends the interaction
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IInteractable_Basic functions")
		bool EndInteract();

	/**
	 * Gets a reference of the data structure of the interactable data
	 * @param InteractableStruct - Reference to the variable to be assignated the data struct
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IInteractable_Basic functions")
		void GetInteractableData_Basic(FInteractableData_Basic& InteractableStruct);

	/**
	 * Sets a new data structure of the interactable data
	 * @param InteractableStruct - New data struct to assing
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IInteractable_Basic functions")
		void SetInteractableData_Basic(FInteractableData_Basic InteractableStruct);
};
