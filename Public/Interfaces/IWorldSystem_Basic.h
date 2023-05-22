// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../GeneralEnumLibrary.h"
#include "../GeneralStructLibrary.h"
#include "IWorldSystem_Basic.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIWorldSystem_Basic : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ALSV4_CPP_API IIWorldSystem_Basic
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * Execute a event depending of the hour of the day and it's wheater
	 * @param Hour - The hour of the day
	 * @param Wheater - The wheater to execute the event, empty for any
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IWorldSystem_Basic functions")
		void ExecuteTimeEvent(int32 Hour, EWeatherType Wheater);

	/**
	 * Gets a reference of the data structure of the world system data
	 * @param WorldSystemStruct - Reference to the variable to be assignated the data struct
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IUseSystem functions")
		void GetUseSystemData(FWorldSystemData_Basic& WorldSystemStruct);

	/**
	 * Sets a new data structure of the world system data
	 * @param WorldSystemStruct - New data struct to assing
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IUseSystem functions")
		void SetUseSystemData(FWorldSystemData_Basic WorldSystemStruct);
};
