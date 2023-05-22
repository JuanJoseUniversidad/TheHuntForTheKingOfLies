// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../GeneralStructLibrary.h"
#include "IDialogueComponent.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIDialogueComponent : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ALSV4_CPP_API IIDialogueComponent
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * Sets the name (if the paramenter its not empty) and the nickname (if the paramenter its not empty) of the owner's dialog
	 * If the name originally had "???" symbolize the initial state then its overriden.
	 * @param Name - The name to set on the owner's full name.
	 * @param Nickname - The nickname to set on the owner's full name. 
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IInteractable_Basic functions")
		void ConstructFullName(FName Name = "", FName Nickname = "");

	/**
	 * Gets the owner's name.
	 * @return The owner's name.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IInteractable_Basic functions")
		FName GetFullName();

	/**
	 * Sets the reference to the actor who triggered the dialogue.
	 * @param Interactor - The actor which starts the dialogue.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IInteractable_Basic functions")
		void SetInteractor(AActor* Interactor);

	/**
	 * Returns the reference of the interactor.
	 * @return The reference of the interacto.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IInteractable_Basic functions")
		AActor* GetInteractor();

	/**
	 * Gets a reference of the data structure of the dialogue component data
	 * @param DialogueComponentStruct - Reference to the variable to be assignated the data struct
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IInteractable_Basic functions")
		void GetDialogueComponentData(FDialogueComponentData& DialogueComponentStruct);

	/**
	 * Sets a new data structure of the dialogue component data
	 * @param DialogueComponentStruct - New data struct to assing
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IInteractable_Basic functions")
		void SetDialogueComponentData(FDialogueComponentData DialogueComponentStruct);
};
