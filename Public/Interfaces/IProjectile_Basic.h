// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../GeneralStructLibrary.h"
#include "IProjectile_Basic.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIProjectile_Basic : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ALSV4_CPP_API IIProjectile_Basic
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * Called when the projectile is fire
	 * @param InitLocation - Vector of the intial location of the projectile
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IProjectile_Basic functions")
		void Fire(FVector InitLocation);

	/**
	 * Gets a reference of the data structure of the projectile data
	 * @param ProjectileStruct - Reference to the variable to be assignated the data struct
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IProjectile_Basic functions")
		void GetProjectileData(FProjectileData& ProjectileStruct);

	/**
	 * Sets a new data structure of the projectile data
	 * @param ProjectileStruct - New data struct to assing
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IProjectile_Basic functions")
		void SetProjectileData(FProjectileData ProjectileStruct);
};
