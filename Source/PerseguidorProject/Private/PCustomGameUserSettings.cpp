// Fill out your copyright notice in the Description page of Project Settings.


#include "PCustomGameUserSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundMix.h"
#include "Sound/SoundClass.h"
#include "Kismet/KismetInternationalizationLibrary.h"

UPCustomGameUserSettings::UPCustomGameUserSettings() {
	MasterVolume = 0.2f;
	SFXVolume = 0.2f;
	UIVolume = 0.2f;
	MusicVolume = 0.2f;
	Fov = 90.0f;
	Gamma = 2.2f;
	MotionBlur = 0;
	Language = FName("en");
}

UPCustomGameUserSettings* UPCustomGameUserSettings::GetCustomUserSettings() {
	return Cast<UPCustomGameUserSettings>(UPCustomGameUserSettings::GetGameUserSettings());
}

void UPCustomGameUserSettings::ApplyNonResolutionSettings() {
	Super::ApplyNonResolutionSettings();
	ApplyMasterVolume();
	ApplySFXVolume();
	ApplyUIVolume();
	ApplyMusicVolume();
	ApplyFov();
	ApplyGamma();
	ApplyMotionBlur();
	ApplyLanguage();
}

void UPCustomGameUserSettings::SetMasterVolume(float NewVolume, UObject* WorldContextObject, USoundMix* SoundMix, USoundClass* SoundClass) {
	MasterVolume = NewVolume;
	WorldContext = WorldContextObject;
	GeneralSoundMix = SoundMix;
	MasterSoundClass = SoundClass;
	ApplyMasterVolume();
}

void UPCustomGameUserSettings::ApplyMasterVolume() {
	if (GeneralSoundMix && MasterSoundClass) {
		UGameplayStatics::SetSoundMixClassOverride(WorldContext, GeneralSoundMix, MasterSoundClass, MasterVolume, 1.0f, 0.01f);
	}
}

float UPCustomGameUserSettings::GetMasterVolume() const {
	return MasterVolume;
}

void UPCustomGameUserSettings::SetSFXVolume(float NewVolume, UObject* WorldContextObject, USoundMix* SoundMix, USoundClass* SoundClassToApply) {
	SFXVolume = NewVolume;
	WorldContext = WorldContextObject;
	GeneralSoundMix = SoundMix;
	SFXSoundClass = SoundClassToApply;
	ApplySFXVolume();
}

void UPCustomGameUserSettings::ApplySFXVolume() {
	if (GeneralSoundMix && MasterSoundClass) {
		UGameplayStatics::SetSoundMixClassOverride(WorldContext, GeneralSoundMix, SFXSoundClass, SFXVolume, 1.0f, 0.01f);
	}
}

float UPCustomGameUserSettings::GetSFXVolume() const {
	return SFXVolume;
}

void UPCustomGameUserSettings::SetUIVolume(float NewVolume, UObject* WorldContextObject, USoundMix* SoundMix, USoundClass* SoundClassToApply) {
	UIVolume = NewVolume;
	WorldContext = WorldContextObject;
	GeneralSoundMix = SoundMix;
	UISoundClass = SoundClassToApply;
	ApplyUIVolume();
}

void UPCustomGameUserSettings::ApplyUIVolume() {
	if (GeneralSoundMix && MasterSoundClass) {
		UGameplayStatics::SetSoundMixClassOverride(WorldContext, GeneralSoundMix, UISoundClass, UIVolume, 1.0f, 0.01f);
	}
}

float UPCustomGameUserSettings::GetUIVolume() const {
	return UIVolume;
}

void UPCustomGameUserSettings::SetMusicVolume(float NewVolume, UObject* WorldContextObject, USoundMix* SoundMix, USoundClass* SoundClassToApply) {
	MusicVolume = NewVolume;
	WorldContext = WorldContextObject;
	GeneralSoundMix = SoundMix;
	MusicSoundClass = SoundClassToApply;
	ApplyMusicVolume();
}

void UPCustomGameUserSettings::ApplyMusicVolume() {
	if (GeneralSoundMix && MasterSoundClass) {
		UGameplayStatics::SetSoundMixClassOverride(WorldContext, GeneralSoundMix, MusicSoundClass, MusicVolume, 1.0f, 0.01f);
	}
}

float UPCustomGameUserSettings::GetMusicVolume() const {
	return MusicVolume;
}

void UPCustomGameUserSettings::SetFov(int32 NewFov) {
	Fov = NewFov;
	ApplyFov();
}

void UPCustomGameUserSettings::ApplyFov() {
	FString Command = "FOV " + Fov;
	GetWorld()->Exec(GetWorld(), *Command);
}

int32 UPCustomGameUserSettings::GetFov() const {
	return Fov;
}

void UPCustomGameUserSettings::SetGamma(float NewGamma) {
	Gamma = NewGamma;
	ApplyGamma();
}

void UPCustomGameUserSettings::ApplyGamma() {
	GEngine->DisplayGamma = Gamma;
}

float UPCustomGameUserSettings::GetGamma() const {
	return Gamma;
}

void UPCustomGameUserSettings::SetMotionBlur(int32 NewMotionBlur) {
	MotionBlur = NewMotionBlur;
	ApplyMotionBlur();
}

void UPCustomGameUserSettings::ApplyMotionBlur() {
	FString Command = "r.MotionBlurQuality  " + MotionBlur;
	GetWorld()->Exec(GetWorld(), *Command);
}

int32 UPCustomGameUserSettings::GetMotionBlur() const {
	return MotionBlur;
}

void UPCustomGameUserSettings::SetLanguage(FName NewLanguage) {
	Language = NewLanguage;
	ApplyLanguage();
}

void UPCustomGameUserSettings::ApplyLanguage() {
	UKismetInternationalizationLibrary::SetCurrentLanguageAndLocale(Language.ToString());
}

FName UPCustomGameUserSettings::GetLanguage() const {
	return Language;
}
