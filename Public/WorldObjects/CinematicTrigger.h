// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interfaces/IWorldSystem_Basic.h"
#include "Components/BoxComponent.h"
#include "../GeneralEnumLibrary.h"
#include "CinematicTrigger.generated.h"

UCLASS()
class ALSV4_CPP_API ACinematicTrigger : public AActor, public IIWorldSystem_Basic
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACinematicTrigger();

	//IWorldSystem_Basic
	virtual void ExecuteTimeEvent_Implementation(int32 Hour, EWeatherType Wheater) override;
	virtual void GetUseSystemData_Implementation(FWorldSystemData_Basic& WorldSystemStruct) override;
	virtual void SetUseSystemData_Implementation(FWorldSystemData_Basic WorldSystemStruct) override;

public:
	/**
	 * Function which plays the sequencer
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CinematicTrigger | Functions")
		void PlaySequencer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	//On overlap begin
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	/* Components */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UBoxComponent* TriggerEvent;

	/* Properties */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		FWorldSystemData_Basic WorldSystemData_Basic;

public:
	bool bEnabledTrigger = true;
	bool bTriggered = false;
};
