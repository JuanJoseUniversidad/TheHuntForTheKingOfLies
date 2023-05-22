// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldObjects/CinematicTrigger.h"

// Sets default values
ACinematicTrigger::ACinematicTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerEvent = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger event"));
	TriggerEvent->SetupAttachment(RootComponent);

	TriggerEvent->OnComponentBeginOverlap.AddDynamic(this, &ACinematicTrigger::OnOverlapBegin);
}

#pragma region WorldSystem_Basic
void ACinematicTrigger::ExecuteTimeEvent_Implementation(int32 Hour, EWeatherType Wheater) {
	if (Hour >= WorldSystemData_Basic.HourActiveFrom && Hour <= WorldSystemData_Basic.HourActiveTo && (WorldSystemData_Basic.ValidWheaters.IsEmpty() || WorldSystemData_Basic.ValidWheaters.Contains(Wheater))) {
		bEnabledTrigger = true;
	}
	else {
		bEnabledTrigger = false;
	}
}

void ACinematicTrigger::GetUseSystemData_Implementation(FWorldSystemData_Basic& WorldSystemStruct) {
	WorldSystemStruct = WorldSystemData_Basic;
}

void ACinematicTrigger::SetUseSystemData_Implementation(FWorldSystemData_Basic WorldSystemStruct) {
	WorldSystemData_Basic = WorldSystemStruct;
}
#pragma endregion

void ACinematicTrigger::PlaySequencer_Implementation() {
}

// Called when the game starts or when spawned
void ACinematicTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACinematicTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACinematicTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (bEnabledTrigger == true && bTriggered == false) {
		bTriggered = true;

		//Plays the sequencer and must set its actor lifetime as long as the duration of the sequencer
		PlaySequencer();

		//Destroy self
		Destroy();
	}
}

