// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../GeneralStructLibrary.h"
#include "../Interfaces/IConsumeComponent.h"
#include "../Interfaces/IConsumableObject_Basic.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Consume_Component.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALSV4_CPP_API UConsume_Component : public UActorComponent, public IIConsumeComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UConsume_Component();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* Function */
	virtual void StartConsuming_Implementation() override;

public:
	/* Interfaces variables */
	//Consumable object basic
	IIConsumableObject_Basic* ConsumableObjectBasic_Interface;
	FConsumableObjectData_Basic ConsumableObjectData_Basic;

public:
	/* Variables */
	UPROPERTY(EditAnywhere, Category = "Consume | Components")
		UStaticMeshComponent* ConsumableAnchor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consume | Properties")
		FConsumeComponentData ConsumeComponentData;

	AActor* ActorConsumable;

private:
	ACharacter* Owner;
};
