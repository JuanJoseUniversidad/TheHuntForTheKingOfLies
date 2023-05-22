// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../GeneralEnumLibrary.h"
#include "../GeneralStructLibrary.h"
#include "IGlobalSoundsManager_Basic.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIGlobalSoundsManager_Basic : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ALSV4_CPP_API IIGlobalSoundsManager_Basic
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * Execute a event for a specific sound of the manager
	 * @param EventName - Name of the event to be played and set as new
	 * @param SoundID - Id of the sound to access
	 * \note This method dont set anything and dont check anything it just execute a soundevent all by itself
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IGlobalSoundsManager_Basic functions")
		void ExecuteSoundEvent(FName EventName, int32 SoundID);

	/**
	 * Execute and set a new base event for a specific sound of the manager
	 * @param EventName - Name of the event to be played and set as new
	 * @param SoundID - Id of the sound to access
	 * \note Its important to say that if a event its on situational mode the new one it will not be played BUT it will be set (for specific cases like a combat while its dawning and the music when the combat finish must be adecuate to the situation)
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IGlobalSoundsManager_Basic functions")
		void ExecuteBaseSoundEvent(FName EventName, int32 SoundID);

	/**
	 * ONLY execute a new event for a specific sound of the manager and set it as situational mode which implies that until the situation it's not finished another base event cannot be played (but it will be assigned)
	 * @param EventName - Name of the event to be played and set as new
	 * @param SoundID - Id of the sound to access
	 * @see void ExecuteBaseSoundEvent(FName EventName, int32 SoundID);
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IGlobalSoundsManager_Basic functions")
		void EnterSituationalSoundEvent(FName EventName, int32 SoundID);

	/**
	 * Execute the base event of the indicated sound to return it to the base state and sets the situational value as false
	 * @param SoundID - Id of the sound to access
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IGlobalSoundsManager_Basic functions")
		void ExitSituationalSoundEvent(int32 SoundID);

	/**
	 * Lock or unlock the sounds to avoid play new events in the system
	 * @param Lock - Indicates if must lock or unlock the system of the given sound
	 * @param SoundID - Id of the sound to access to block his events
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IGlobalSoundsManager_Basic functions")
		void LockSoundSystem(bool Lock, int32 SoundID);

	/**
	 * Gets a reference of the data structure of the global sound manager system data
	 * @param GlobalSoundsManagerSystemStruct - Reference to the variable to be assignated the data struct
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IGlobalSoundsManager_Basic functions")
		void GetGlobalSoundsManagerSystemData_Basic(FGlobalSoundsManagerSystemData_Basic& GlobalSoundsManagerSystemStruct);

	/**
	 * Sets a new data structure of the global sound manager system data
	 * @param GlobalSoundsManagerSystemStruct - New data struct to assing
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IGlobalSoundsManager_Basic functions")
		void SetGlobalSoundsManagerSystemData_Basic(FGlobalSoundsManagerSystemData_Basic GlobalSoundsManagerSystemStruct);
};
