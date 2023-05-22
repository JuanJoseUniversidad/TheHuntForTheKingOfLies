// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../GeneralStructLibrary.h"
#include "IUseSystem.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIUseSystem : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ALSV4_CPP_API IIUseSystem
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * Called when character use
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IUseSystem functions")
		void Use();

	/**
	 * Gets a reference of the data structure of the use system data
	 * @param UseSystemStruct - Reference to the variable to be assignated the data struct
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IUseSystem functions")
		void GetUseSystemData(FUseSystemData& UseSystemStruct);

	/**
	 * Sets a new data structure of the use system data
	 * @param UseSystemStruct - New data struct to assing
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IUseSystem functions")
		void SetUseSystemData(FUseSystemData UseSystemStruct);
};
