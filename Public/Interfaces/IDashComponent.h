// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../GeneralStructLibrary.h"
#include "GenericPlatform/GenericPlatformProcess.h"
#include "Containers/Array.h"
#include "../Library/ALSCharacterEnumLibrary.h"
#include "IDashComponent.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIDashComponent : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ALSV4_CPP_API IIDashComponent
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * Starts the dash adding an impulse to the owner
	 * @param IsCrouch - If its crouching
	 * @param AlternativeDirection - The alternative direction wich want to lauch, if its 0 gets the last input of the player
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IDashComponent functions")
		void StartDash(bool IsCrouch, EALSEightDirection AlternativeDirection = EALSEightDirection::None);

	/**
	 * Ends the dash renabling everything
	 * @param ReuseDashDelay - Time to use the dash again
	 * @param EnableAnimationsDelay - Time to renable the animations of the owner
	 * @param VisiblePlayerDelay - Time to set the player visible again
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IDashComponent functions")
		void EndDash(float ReuseDashDelay, float EnableAnimationsDelay, float VisiblePlayerDelay);

	/**
	 * Gets a reference of the data structure of the dash component data
	 * @param DashComponentStruct - Reference to the variable to be assignated the data struct
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IDashComponent functions")
		void GetDashComponentData(FDashComponentData& DashComponentStruct);

	/**
	 * Sets a new data structure of the dash component data
	 * @param DashComponentStruct - New data struct to assing
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IDashComponent functions")
		void SetDashComponentData(FDashComponentData DashComponentStruct);
};
