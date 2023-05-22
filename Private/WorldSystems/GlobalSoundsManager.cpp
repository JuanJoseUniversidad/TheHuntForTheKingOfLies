// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldSystems/GlobalSoundsManager.h"

// Sets default values
AGlobalSoundsManager::AGlobalSoundsManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

#pragma region WorldSystem_Basic
void AGlobalSoundsManager::ExecuteTimeEvent_Implementation(int32 Hour, EWeatherType Wheater) {
	for (auto& eventTime : SoundEventByTimeDay) {
		if (Hour >= eventTime.Value.HourActiveFrom && Hour <= eventTime.Value.HourActiveTo && (eventTime.Value.ValidWheaters.IsEmpty() || eventTime.Value.ValidWheaters.Contains(Wheater))) {
			ExecuteBaseSoundEvent_Implementation(eventTime.Key, eventTime.Value.SoundID);
		}
	}
}

void AGlobalSoundsManager::GetUseSystemData_Implementation(FWorldSystemData_Basic& WorldSystemStruct) {
	WorldSystemStruct = WorldSystemData_Basic;
}

void AGlobalSoundsManager::SetUseSystemData_Implementation(FWorldSystemData_Basic WorldSystemStruct) {
	WorldSystemData_Basic = WorldSystemStruct;
}
#pragma endregion

#pragma region GoblarSoundsManager_Basic
void AGlobalSoundsManager::ExecuteSoundEvent_Implementation(FName EventName, int32 SoundID) {
	if (SoundID < GlobalSoundsManagerSystemData_Basic.AudiosComponent.Num()) {
		if (SoundsLocked[SoundID] == false && CurrentExecuteEvent[SoundID] != EventName) {
			GlobalSoundsManagerSystemData_Basic.AudiosComponent[SoundID]->SetTriggerParameter(EventName);
			CurrentExecuteEvent[SoundID] = EventName;
		}
	}
}

void AGlobalSoundsManager::ExecuteBaseSoundEvent_Implementation(FName EventName, int32 SoundID) {
	if (SoundID < GlobalSoundsManagerSystemData_Basic.AudiosComponent.Num()) {
		if (SoundsLocked[SoundID] == false && CurrentExecuteEvent[SoundID] != EventName && GlobalSoundsManagerSystemData_Basic.bSoundsEnterSituationalMode[SoundID] == false) {
			//Execute sound event
			GlobalSoundsManagerSystemData_Basic.AudiosComponent[SoundID]->SetTriggerParameter(EventName);
			CurrentExecuteEvent[SoundID] = EventName;
		}
		SoundsBaseEvents[SoundID] = EventName;
	}
}

void AGlobalSoundsManager::EnterSituationalSoundEvent_Implementation(FName EventName, int32 SoundID) {
	if (SoundID < GlobalSoundsManagerSystemData_Basic.AudiosComponent.Num()) {
		GlobalSoundsManagerSystemData_Basic.bSoundsEnterSituationalMode[SoundID] = true;

		if (SoundsLocked[SoundID] == false && CurrentExecuteEvent[SoundID] != EventName) {
			//Execute sound event
			GlobalSoundsManagerSystemData_Basic.AudiosComponent[SoundID]->SetTriggerParameter(EventName);
			CurrentExecuteEvent[SoundID] = EventName;
		}
	}
}

void AGlobalSoundsManager::ExitSituationalSoundEvent_Implementation(int32 SoundID) {
	if (SoundID < GlobalSoundsManagerSystemData_Basic.AudiosComponent.Num()) {
		GlobalSoundsManagerSystemData_Basic.bSoundsEnterSituationalMode[SoundID] = false;
		//Execute sound event
		GlobalSoundsManagerSystemData_Basic.AudiosComponent[SoundID]->SetTriggerParameter(SoundsBaseEvents[SoundID]);
		CurrentExecuteEvent[SoundID] = SoundsBaseEvents[SoundID];
	}
}

void AGlobalSoundsManager::LockSoundSystem_Implementation(bool Lock, int32 SoundID) {
	if (SoundID < GlobalSoundsManagerSystemData_Basic.AudiosComponent.Num()) {
		SoundsLocked[SoundID] = Lock;
	}
}

void AGlobalSoundsManager::GetGlobalSoundsManagerSystemData_Basic_Implementation(FGlobalSoundsManagerSystemData_Basic& GlobalSoundsManagerSystemStruct) {
	GlobalSoundsManagerSystemStruct = GlobalSoundsManagerSystemData_Basic;
}

void AGlobalSoundsManager::SetGlobalSoundsManagerSystemData_Basic_Implementation(FGlobalSoundsManagerSystemData_Basic GlobalSoundsManagerSystemStruct) {
	GlobalSoundsManagerSystemData_Basic = GlobalSoundsManagerSystemStruct;
}
#pragma endregion

// Called when the game starts or when spawned
void AGlobalSoundsManager::BeginPlay()
{
	Super::BeginPlay();
	
	//Spawn the audios and assign the audios components
	for (auto& Sound : GlobalSoundsManagerSystemData_Basic.Sounds) {
		GlobalSoundsManagerSystemData_Basic.AudiosComponent.Add(UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Sound.Key, FVector(0, 0, 0)));
		GlobalSoundsManagerSystemData_Basic.bSoundsEnterSituationalMode.Add(false);
	}

	//Gets the list of base events
	GlobalSoundsManagerSystemData_Basic.Sounds.GenerateValueArray(SoundsBaseEvents);	
	//Initialize the arrays
	CurrentExecuteEvent.Init("", GlobalSoundsManagerSystemData_Basic.Sounds.Num());
	SoundsLocked.Init(false, GlobalSoundsManagerSystemData_Basic.Sounds.Num());
}

// Called every frame
void AGlobalSoundsManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

