// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../GeneralStructLibrary.h"
#include "IEquipableObject_Basic.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIEquipableObject_Basic : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ALSV4_CPP_API IIEquipableObject_Basic
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/**
	 * Called when the equipped object is tried to being used
	 * @param UseMode - The mode to use the object if has multiple types of use
	 * @return The time of usage, -1 if couldnt use it
	 * @note It checks if the UseMode its out of bounds of the array of possible actions to perform, in tath case clamps the value to 0 or the last action, depending of the value of the UseMode
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IEquipableObject_Basic functions")
		float TryToUse(int32 UseMode = 0);

	/**
	 * Called when the equipped object is used
	 * @return If use the object susccessfully
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IEquipableObject_Basic functions")
		bool Use();

	/**
	 * Sets the mesh of the equipable object visible or not
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IEquipableObject_Basic functions")
		void SetVisibleMesh(bool Visible);

	/**
	 * Plays the sound of the equipable object when is ready to use, generally when the character is aiming
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IEquipableObject_Basic functions")
		void PlayReadySound();

	/**
	 * Stops the sound of the equipable object when is ready to use, generally when the character stop aiming
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IEquipableObject_Basic functions")
		void StopReadySound();

	/**
	 * Gets a reference of the data structure of the equipable object data
	 * @param EquipableObjectStruct - Reference to the variable to be assignated the data struct
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IEquipableObject_Basic functions")
		void GetEquipableObjectData_Basic(FEquipableObjectData_Basic& EquipableObjectStruct);

	/**
	 * Sets a new data structure of the equipable object data
	 * @param EquipableObjectStruct - New data struct to assing
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IEquipableObject_Basic functions")
		void SetEquipableObjectData_Basic(FEquipableObjectData_Basic EquipableObjectStruct);
};
