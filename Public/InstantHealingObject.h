// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GeneralStructLibrary.h"
#include "Interfaces/IConsumableObject_Basic.h"
#include "Interfaces/IHealthComponent.h"
#include "Character/ALSCharacter.h"
#include "Character/ALSBaseCharacter.h"
#include "InstantHealingObject.generated.h"

UCLASS()
class ALSV4_CPP_API AInstantHealingObject : public AActor, public IIConsumableObject_Basic
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInstantHealingObject();

	//IIConsumableObject_Basic
	virtual bool Consume_Implementation(AActor* Actor) override;
	virtual void GetConsumableObjectData_Basic_Implementation(FConsumableObjectData_Basic& ConsumableObjectStruct) override;
	virtual void SetConsumableObjectData_Basic_Implementation(FConsumableObjectData_Basic ConsumableObjectStruct) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	//Properties
	UPROPERTY(EditAnywhere, Category = "Properties")
		FConsumableObjectData_Basic ConsumableObjectData_Basic;

	UPROPERTY(EditAnywhere, Category = "Properties")
		float CureHeal = 100.0f;
};
