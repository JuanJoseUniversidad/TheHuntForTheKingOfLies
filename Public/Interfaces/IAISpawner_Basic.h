// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../GeneralStructLibrary.h"
#include "IAISpawner_Basic.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIAISpawner_Basic : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ALSV4_CPP_API IIAISpawner_Basic
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * Spawns the AI's
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IAISpawner_Basic functions")
		void Spawn();

	/**
	 * Despawns the AI's
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IAISpawner_Basic functions")
		void Despawn();

	/**
	 * Gets a reference of the data structure of the world system basic data
	 * @param AISpawnerStruct - Reference to the variable to be assignated the data struct
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IAISpawner_Basic functions")
		void GetAISpawnerData_Basic(FAISpawnerData_Basic& AISpawnerStruct);

	/**
	 * Sets a new data structure of the world system basic data
	 * @param WorldSystemStruct - New data struct to assing
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IAISpawner_Basic functions")
		void SetAISpawnerData_Basic(FAISpawnerData_Basic AISpawnerStruct);

};
