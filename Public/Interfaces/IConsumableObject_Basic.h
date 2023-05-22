// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../GeneralStructLibrary.h"
#include "IConsumableObject_Basic.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIConsumableObject_Basic : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ALSV4_CPP_API IIConsumableObject_Basic
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * Consume the object
	 * @return If the consume was susccessfully
	 * @param Actor - The actor which will recibe the consume effect
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IConsumableObject_Basic functions")
		bool Consume(AActor* Actor);

	/**
	 * Gets a reference of the data structure of the consumable object data
	 * @param ConsumableObjectStruct - Reference to the variable to be assignated the data struct
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IConsumableObject_Basic functions")
		void GetConsumableObjectData_Basic(FConsumableObjectData_Basic& ConsumableObjectStruct);

	/**
	 * Sets a new data structure of the equipable object data
	 * @param ConsumableObjectStruct - New data struct to assing
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IConsumableObject_Basic functions")
		void SetConsumableObjectData_Basic(FConsumableObjectData_Basic ConsumableObjectStruct);
};
