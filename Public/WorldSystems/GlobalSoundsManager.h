// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interfaces/IWorldSystem_Basic.h"
#include "../Interfaces/IGlobalSoundsManager_Basic.h"
#include "Kismet/GameplayStatics.h"
#include "GlobalSoundsManager.generated.h"

USTRUCT(BlueprintType)
struct FSoundEventByTimeDay {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		int32 HourActiveFrom = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		int32 HourActiveTo = 23;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		TArray<EWeatherType> ValidWheaters;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		int32 SoundID;
};

UCLASS()
class ALSV4_CPP_API AGlobalSoundsManager : public AActor, public IIWorldSystem_Basic, public IIGlobalSoundsManager_Basic
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGlobalSoundsManager();

	//IIWorldSystem_Basic
	virtual void ExecuteTimeEvent_Implementation(int32 Hour, EWeatherType Wheater) override;
	virtual void GetUseSystemData_Implementation(FWorldSystemData_Basic& WorldSystemStruct) override;
	virtual void SetUseSystemData_Implementation(FWorldSystemData_Basic WorldSystemStruct) override;

	//IIGlobalSoundsManager_Basic
	virtual void ExecuteSoundEvent_Implementation(FName EventName, int32 SoundID) override;
	virtual void ExecuteBaseSoundEvent_Implementation(FName EventName, int32 SoundID) override;
	virtual void EnterSituationalSoundEvent_Implementation(FName EventName, int32 SoundID) override;
	virtual void ExitSituationalSoundEvent_Implementation(int32 SoundID) override;
	virtual void LockSoundSystem_Implementation(bool Lock, int32 SoundID) override;
	virtual void GetGlobalSoundsManagerSystemData_Basic_Implementation(FGlobalSoundsManagerSystemData_Basic& GlobalSoundsManagerSystemStruct) override;
	virtual void SetGlobalSoundsManagerSystemData_Basic_Implementation(FGlobalSoundsManagerSystemData_Basic GlobalSoundsManagerSystemStruct) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	//Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		FGlobalSoundsManagerSystemData_Basic GlobalSoundsManagerSystemData_Basic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		FWorldSystemData_Basic WorldSystemData_Basic;

	//Key = event name, Value = From which hour execute and its wheater(empty for any) the event and the sound ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		TMap<FName, FSoundEventByTimeDay> SoundEventByTimeDay;

private:
	//Current event executed to avoid rexecute it
	TArray<FName> CurrentExecuteEvent;
	//List of base events of each audiocomponent to return when a situational event pass
	TArray<FName> SoundsBaseEvents;
	//Locked sounds to avoid be trigger any event
	TArray<bool> SoundsLocked;
};
