// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../GeneralStructLibrary.h"
#include "IAimSystem.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIAimSystem : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ALSV4_CPP_API IIAimSystem
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * Called when character aim
	 * @param Aiming - Bool if its aiming
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IAimSystem functions")
		void Aim(bool bAiming);

	/**
	 * Gets a reference of the data structure of the aim system data
	 * @param AimSystemStruct - Reference to the variable to be assignated the data struct
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IAimSystem functions")
		void GetAimSystemData(FAimSystemData& AimSystemStruct);

	/**
	 * Sets a new data structure of the aim system data
	 * @param AimSystemStruct - New data struct to assing
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IAimSystem functions")
		void SetAimSystemData(FAimSystemData AimSystemStruct);
};
