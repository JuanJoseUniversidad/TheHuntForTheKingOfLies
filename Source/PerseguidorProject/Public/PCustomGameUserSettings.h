// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "PCustomGameUserSettings.generated.h"

class USoundMix;
class USoundClass;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class PERSEGUIDORPROJECT_API UPCustomGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	UPCustomGameUserSettings();

	UFUNCTION(BlueprintCallable)
		static UPCustomGameUserSettings* GetCustomUserSettings();

	virtual void ApplyNonResolutionSettings() override;

public:
	/**
	 * Sets a new volume level and applies it
	 * @param NewVolume - The new volume volume
	 * @param WorldContextObject - The object context form which the volume change its applied
	 * @param SoundMix - The general project sound mix use to applied the volume level
	 * @param SoundClassToApply - Sound class to apply the new volume level
	 */
	UFUNCTION(BlueprintCallable)
		void SetMasterVolume(float NewVolume, UObject* WorldContextObject, USoundMix* SoundMix, USoundClass* SoundClassToApply);

	/**
	 * Applies the volume with the stored value
	 */
	void ApplyMasterVolume();

	/**
	 * Gets the volume level
	 * @return The volume volume
	 */
	UFUNCTION(BlueprintPure)
		float GetMasterVolume() const;
	


	/**
	 * Sets a new volume level and applies it
	 * @param NewVolume - The new volume volume
	 * @param WorldContextObject - The object context form which the volume change its applied
	 * @param SoundMix - The general project sound mix use to applied the volume level
	 * @param SoundClassToApply - Sound class to apply the new volume level
	 */
	UFUNCTION(BlueprintCallable)
		void SetSFXVolume(float NewVolume, UObject* WorldContextObject, USoundMix* SoundMix, USoundClass* SoundClassToApply);

	/**
	 * Applies the volume with the stored value
	 */
	void ApplySFXVolume();

	/**
	 * Gets the volume level
	 * @return The volume volume
	 */
	UFUNCTION(BlueprintPure)
		float GetSFXVolume() const;



	/**
	 * Sets a new volume level and applies it
	 * @param NewVolume - The new volume volume
	 * @param WorldContextObject - The object context form which the volume change its applied
	 * @param SoundMix - The general project sound mix use to applied the volume level
	 * @param SoundClassToApply - Sound class to apply the new volume level
	 */
	UFUNCTION(BlueprintCallable)
		void SetUIVolume(float NewVolume, UObject* WorldContextObject, USoundMix* SoundMix, USoundClass* SoundClassToApply);

	/**
	 * Applies the volume with the stored value
	 */
	void ApplyUIVolume();

	/**
	 * Gets the volume level
	 * @return The volume volume
	 */
	UFUNCTION(BlueprintPure)
		float GetUIVolume() const;



	/**
	 * Sets a new volume level and applies it
	 * @param NewVolume - The new volume volume
	 * @param WorldContextObject - The object context form which the volume change its applied
	 * @param SoundMix - The general project sound mix use to applied the volume level
	 * @param SoundClassToApply - Sound class to apply the new volume level
	 */
	UFUNCTION(BlueprintCallable)
		void SetMusicVolume(float NewVolume, UObject* WorldContextObject, USoundMix* SoundMix, USoundClass* SoundClassToApply);

	/**
	 * Applies the volume with the stored value
	 */
	void ApplyMusicVolume();

	/**
	 * Gets the volume level
	 * @return The volume volume
	 */
	UFUNCTION(BlueprintPure)
		float GetMusicVolume() const;


	/**
	 * Sets the new Field Of View and applies it
	 * @param NewFov - The new field of view
	 */
	UFUNCTION(BlueprintCallable)
		void SetFov(int32 NewFov);

	/**
	 * Applies the field of view with the stored value
	 */
	void ApplyFov();

	/**
	 * Gets the fov
	 * @return The field of view
	 */
	UFUNCTION(BlueprintPure)
		int32 GetFov() const;


	/**
	 * Sets the new gamma and applies it
	 * @param NewGamma - The new gamma
	 */
	UFUNCTION(BlueprintCallable)
		void SetGamma(float NewGamma);

	/**
	 * Applies the gamma with the stored value
	 */
	void ApplyGamma();

	/**
	 * Gets the gamma
	 * @return The gamma
	 */
	UFUNCTION(BlueprintPure)
		float GetGamma() const;



	/**
	 * Sets the new motion blur and applies it
	 * @param NewGamma - The new motion blur
	 */
	UFUNCTION(BlueprintCallable)
		void SetMotionBlur(int32 NewMotionBlur);

	/**
	 * Applies the gamma with the stored value
	 */
	void ApplyMotionBlur();

	/**
	 * Gets the motion blur
	 * @return The motion blur
	 */
	UFUNCTION(BlueprintPure)
		int32 GetMotionBlur() const;



	/**
	 * Sets the new language and applies it
	 * @param NewGamma - The new motion blur
	 */
	UFUNCTION(BlueprintCallable)
		void SetLanguage(FName NewLanguage);

	/**
	 * Applies the language with the stored value
	 */
	void ApplyLanguage();

	/**
	 * Gets the language
	 * @return The languge
	 */
	UFUNCTION(BlueprintPure)
		FName GetLanguage() const;


protected:
	UPROPERTY(Config)
		float MasterVolume;

	UPROPERTY(Config)
		float SFXVolume;

	UPROPERTY(Config)
		float UIVolume;

	UPROPERTY(Config)
		float MusicVolume;

	UPROPERTY(Config)
		float Gamma;

	UPROPERTY(Config)
		int32 Fov;

	UPROPERTY(Config)
		int32 MotionBlur;

	UPROPERTY(Config)
		FName Language;

	/*World context for change the audio*/
	UObject* WorldContext;

	/*The sound mix which will change the volume levels*/
	USoundMix* GeneralSoundMix;

	USoundClass* MasterSoundClass;
	USoundClass* SFXSoundClass;
	USoundClass* UISoundClass;
	USoundClass* MusicSoundClass;


};
