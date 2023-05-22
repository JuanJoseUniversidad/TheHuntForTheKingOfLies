// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../Interfaces/IGlobalSoundsManager_Basic.h"
#include "GlobalSoundsEventsTrigger.generated.h"

UENUM(BlueprintType)
enum class ETriggerEventType : uint8 {
	Event,
	BaseEvent,
	EnterSituationalEvent,
	ExitSituationalEvent
};

USTRUCT(BlueprintType)
struct FTriggerEventData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		int32 SoundID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		FName EventName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		ETriggerEventType EventType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		bool BlockGlobalSoundsManager = true;
};


UCLASS()
class ALSV4_CPP_API AGlobalSoundsEventsTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGlobalSoundsEventsTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//On overlap begin
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//On overlap end
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	/* Functions */
	/**
	 * Execute all the events from a given list and locks or unlocks the GlobalSoundsManager if procee
	 * @param TriggerEvents - The list of events to execute
	 */
	void ExecuteEvents(TArray<FTriggerEventData>& TriggerEvents);

public:
	/* Components */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UBoxComponent* TriggerEvent;

	/* Properties */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		TArray<FTriggerEventData> TriggerEventsEnter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		TArray<FTriggerEventData> TriggerEventsExit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", AdvancedDisplay, meta = (MustImplement = "IGlobalSoundsManager_Basic"))
		AActor* SoundsManager;

private:
	/* Properties */
	IIGlobalSoundsManager_Basic* SoundsManager_Interface;
};
