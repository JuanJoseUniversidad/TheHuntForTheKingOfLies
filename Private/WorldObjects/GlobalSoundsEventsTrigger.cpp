// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldObjects/GlobalSoundsEventsTrigger.h"

// Sets default values
AGlobalSoundsEventsTrigger::AGlobalSoundsEventsTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Set the channel to collide with only pickable objects!!
	TriggerEvent = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger event"));
	TriggerEvent->SetupAttachment(RootComponent);
	
	//Sets the trigger
	TriggerEvent->OnComponentBeginOverlap.AddDynamic(this, &AGlobalSoundsEventsTrigger::OnOverlapBegin);
	TriggerEvent->OnComponentEndOverlap.AddDynamic(this, &AGlobalSoundsEventsTrigger::OnOverlapEnd);
}

// Called when the game starts or when spawned
void AGlobalSoundsEventsTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	//If there is no manager assigned then gets the first one
	if (!SoundsManager) {
		TArray<AActor*> SystemsFound;
		UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UIGlobalSoundsManager_Basic::StaticClass(), SystemsFound);
		if (SystemsFound.Num() > 0) {
			SoundsManager = SystemsFound[0];
		}
	}
	
	//Gets its interface
	SoundsManager_Interface = Cast<IIGlobalSoundsManager_Basic>(SoundsManager);
}

// Called every frame
void AGlobalSoundsEventsTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGlobalSoundsEventsTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	ExecuteEvents(TriggerEventsEnter);
}

void AGlobalSoundsEventsTrigger::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	ExecuteEvents(TriggerEventsExit);
}

void AGlobalSoundsEventsTrigger::ExecuteEvents(TArray<FTriggerEventData>& TriggerEvents) {
	for(auto& eventElem : TriggerEvents){
		switch (eventElem.EventType) {
		case ETriggerEventType::Event:
			SoundsManager_Interface->Execute_ExecuteSoundEvent(SoundsManager, eventElem.EventName, eventElem.SoundID);
			SoundsManager_Interface->Execute_LockSoundSystem(SoundsManager, eventElem.BlockGlobalSoundsManager, eventElem.SoundID);//Locks the system
			break;
		case ETriggerEventType::BaseEvent:
			SoundsManager_Interface->Execute_ExecuteBaseSoundEvent(SoundsManager, eventElem.EventName, eventElem.SoundID);
			SoundsManager_Interface->Execute_LockSoundSystem(SoundsManager, eventElem.BlockGlobalSoundsManager, eventElem.SoundID);//Locks the system
			break;
		case ETriggerEventType::EnterSituationalEvent:
			SoundsManager_Interface->Execute_EnterSituationalSoundEvent(SoundsManager, eventElem.EventName, eventElem.SoundID);
			SoundsManager_Interface->Execute_LockSoundSystem(SoundsManager, eventElem.BlockGlobalSoundsManager, eventElem.SoundID);//Locks the system
			break;
		case ETriggerEventType::ExitSituationalEvent:
			SoundsManager_Interface->Execute_LockSoundSystem(SoundsManager, false, eventElem.SoundID);//Unlocks the system
			SoundsManager_Interface->Execute_ExitSituationalSoundEvent(SoundsManager, eventElem.SoundID);
			break;
		default:
			break;
		}
	}
}

