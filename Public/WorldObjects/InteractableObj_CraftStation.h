// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "../Interfaces/IWorldSystem_Basic.h"
#include "../Interfaces/IInteractable_Basic.h"
#include "../Interfaces/ICraftKnowledgeSystem.h"

#include "../GeneralStructLibrary.h"

#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimMontage.h"
#include "../Character/ALSCharacter.h"
#include "InteractableObj_CraftStation.generated.h"

UCLASS()
class ALSV4_CPP_API AInteractableObj_CraftStation : public AActor, public IIWorldSystem_Basic, public IIInteractable_Basic
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableObj_CraftStation();

	//IWorldSystem_Basic
	virtual void ExecuteTimeEvent_Implementation(int32 Hour, EWeatherType Wheater) override;
	virtual void GetUseSystemData_Implementation(FWorldSystemData_Basic& WorldSystemStruct) override;
	virtual void SetUseSystemData_Implementation(FWorldSystemData_Basic WorldSystemStruct) override;

	//IInteractable_Basic
	virtual bool Interact_Implementation(AActor* Interactor = nullptr) override;
	virtual bool EndInteract_Implementation() override;
	virtual void GetInteractableData_Basic_Implementation(FInteractableData_Basic& InteractableStruct) override;
	virtual void SetInteractableData_Basic_Implementation(FInteractableData_Basic InteractableStruct) override;

public:
	/**
	 * Function which creates the craft station interface with a blueprint layer
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CraftStation | Functions")
		void CreateCraftstationInterface();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	//Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
		TObjectPtr <USceneComponent> SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
		TObjectPtr <UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
		TObjectPtr <USkeletalMeshComponent> WeaponSkeletalMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
		TObjectPtr <USkeletalMeshComponent> PlaceActorSkeletalComponent;


	//Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		FInteractableData_Basic InteractableObject_Basic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		FWorldSystemData_Basic WorldSystemData_Basic;

	//The Key is the crafteable item and the value are the required objects
	UPROPERTY(BlueprintReadWrite, Category = "Properties")
		TArray<FCrafteableObjectData> CraftingList;

	UPROPERTY(EditAnywhere, Category = "Properties")
		UAnimMontage* PlaceActorMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		float TimeToApplyEffects = 1.0f;


private:
	bool bCanBeInteracted = true;

	FCraftKnowledgeSystemData CraftKnowledgeSystemDataRef;

	FTimerHandle TimerHandleEffects;
	FTimerHandle TimerHandleRenable;
};
