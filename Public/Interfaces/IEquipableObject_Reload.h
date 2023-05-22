// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../GeneralStructLibrary.h"
#include "IEquipableObject_Reload.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIEquipableObject_Reload : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ALSV4_CPP_API IIEquipableObject_Reload
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/**
	 * Reloads the magazine(fire arms) or the quiver(for the bow) or the oil(for a lanter) for example of the equippable object
	 * @return If the reload was susccessfully
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IEquipableObject_Reload functions")
		bool Reload();

	/**
	 * Sets the load ready to be use, for example the gun chamber for the firearms, an arrow por the bows or the type of oil for an lanter
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IEquipableObject_Reload functions")
		void SetReadyLoad();

	/**
	 * Sets the type of load to be ready
	 * @return The class assigned to the slot, if there is no assigned slot then return nullptr or if the magazine is full
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IEquipableObject_Reload functions")
		TSubclassOf<AActor> SetReadyLoadType(int32 Slot);

	/**
	 * Sets the types of loads to be selectable when the equipped object its reloading a specific type
	 * @note As an example with this you can set as quick key the types of ammo that the player can reload into his gun
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IEquipableObject_Reload functions")
		void SetLoadsTypesToReload(int32 Slot, TSubclassOf<AActor> Type);

	/**
	 * Check the status of the equipped object
	 * @return If the status its correct or not
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IEquipableObject_Reload functions")
		bool CheckStatus();

	/**
	 * Gets a reference of the data structure of the equipable object data
	 * @param EquipableObjectStruct - Reference to the variable to be assignated the data struct
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IEquipableObject_Reload functions")
		void GetEquipableObjectData_Reload(FEquipableObjectData_Reload& EquipableObjectStruct);

	/**
	 * Sets a new data structure of the equipable object data
	 * @param EquipableObjectStruct - New data struct to assing
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IEquipableObject_Reload functions")
		void SetEquipableObjectData_Reload(FEquipableObjectData_Reload EquipableObjectStruct);
};
