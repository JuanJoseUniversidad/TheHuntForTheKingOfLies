// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../GeneralStructLibrary.h"
#include "ICraftKnowledgeSystem.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UICraftKnowledgeSystem : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ALSV4_CPP_API IICraftKnowledgeSystem
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * Learns a new craft item recipe
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ICraftKnowledgeSystem functions")
		void Learn(FCrafteableObjectData Recipe);

	/**
	 * Forget a learned craft item recipe
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ICraftKnowledgeSystem functions")
		void Forget(FCrafteableObjectData Recipe);

	/**
	 * Gets a reference of the data structure of the craft knowledge system data
	 * @param CraftKnowledgeSystemStruct - Reference to the variable to be assignated the data struct
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ICraftKnowledgeSystem functions")
		void GetCraftKnowledgeSystemData(FCraftKnowledgeSystemData& CraftKnowledgeSystemStruct);

	/**
	 * Sets a new data structure of the craft knowledge system data
	 * @param CraftKnowledgeSystemStruct - New data struct to assing
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ICraftKnowledgeSystem functions")
		void SetCraftKnowledgeSystemData(FCraftKnowledgeSystemData CraftKnowledgeSystemStruct);
};
