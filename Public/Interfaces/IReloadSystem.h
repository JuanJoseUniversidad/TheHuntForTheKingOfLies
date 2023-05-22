// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../GeneralStructLibrary.h"
#include "IReloadSystem.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIReloadSystem : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ALSV4_CPP_API IIReloadSystem
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * Called when character enters on reload mode
	 * @param bReloadMode - If it has entered on reload mode or not
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IReloadSystem functions")
		void ReloadMode(bool bReloadMode);

	/**
	 * Called when character enters is reloading
	 * @param bReloading - If its performing the reloading action
	 * @param TypeOfProjectile - The type of projectile to reload
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IReloadSystem functions")
		void Reloading(bool bReloading, int32 TypeOfProjectile);

	/**
	 * Gets a reference of the data structure of the reload system data
	 * @param ReloadSystemStruct - Reference to the variable to be assignated the data struct
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IReloadSystem functions")
		void GetReloadSystemData(FReloadSystemData& ReloadSystemStruct);

	/**
	 * Sets a new data structure of the reload system data
	 * @param ReloadSystemStruct - New data struct to assing
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IReloadSystem functions")
		void SetReloadSystemData(FReloadSystemData ReloadSystemStruct);
};
