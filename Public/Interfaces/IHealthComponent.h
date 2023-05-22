// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../GeneralStructLibrary.h"
#include "IHealthComponent.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIHealthComponent : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ALSV4_CPP_API IIHealthComponent
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * Adds life
	 * @param Life - The amount of life which wants to heal
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IHealthComponent functions")
		void AddHealth(float Life);

	/**
	 * Revives the player and resets it
	 * @return If the revival was susscessful
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IHealthComponent functions")
		bool Revive();

	/**
	 * Damage when its hitted by external factors like a projectile
	 * @param Damage - Damage to inflict
	 * @param Hit - The hit result of the impact
	 * @param BoneImpulse - Impulse to apply to the affected actor for a natural reaction
	 * @param KnockbackImpulse - Implse apply to the integrity of the character
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IHealthComponent functions")
		void HitDamage(float Damage, const FHitResult& Hit, double BoneImpulse = 5000.0, double KnockbackImpulse = 5000.0);

	/**
	 * Damage while its falling
	 * @param Damage - Damage to inflict
	 * @param Hit - The hit result of the impact
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "IHealthComponent functions")
		void FallDamage(float Damage, const FHitResult& Hit);

	/**
	 * Gets a reference of the data structure of the health component data
	 * @param HealthComponentStruct - Reference to the variable to be assignated the data struct
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IHealthComponent functions")
		void GetHealthComponentData(FHealthComponentData& HealthComponentStruct);

	/**
	 * Sets a new data structure of the health component data
	 * @param HealthComponentStruct - New data struct to assing
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IHealthComponent functions")
		void SetHealthComponentData(FHealthComponentData HealthComponentStruct);
};
