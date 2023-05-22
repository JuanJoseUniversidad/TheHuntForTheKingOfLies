// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "Components/Inventory_Component.h"
#include "./Character/ALSCharacter.h"
#include "GeneralStructLibrary.h"
#include "Interfaces/IPickableObject_Basic.h"
#include "Interfaces/IInventoryComponent.h"
#include "Sound/SoundCue.h"
#include "PickableActor.generated.h"

UCLASS()
class ALSV4_CPP_API APickableActor : public AActor, public IIPickableObject_Basic
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickableActor();

	//IIPickableObject_Basic
	virtual bool Pick_Implementation(AActor* Actor) override;
	virtual void GetPickableObjectData_Basic_Implementation(FPickableObjectData_Basic& PickableObjectStruct) override;
	virtual void SetPickableObjectData_Basic_Implementation(FPickableObjectData_Basic PickableObjectStruct) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	//Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
		TObjectPtr <UStaticMeshComponent> MeshComponent;

	//Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		FPickableObjectData_Basic PickableObjectData_Basic;


private:
	bool bCanBePicked = true;

	FTimerHandle TimerHandleRegenerate;
};
