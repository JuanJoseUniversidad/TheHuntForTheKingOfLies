// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "../Interfaces/IInteractComponent.h"
#include "../Interfaces/IInteractable_Basic.h"
#include "../GeneralStructLibrary.h"
#include "Interact_Component.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class ALSV4_CPP_API UInteract_Component : public UActorComponent, public IIInteractComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteract_Component();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//On overlap begin
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//On overlap end
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/**
	 * Fuction wich recives the candidate actor to interact with and gets its name and type of action to update the action notification of the HUD, if its nullptr means there is no actor
	 * @param Actor - Candidate actor to update the HUD
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|InteractComponent")
		void UpdateHUDBP(AActor* Actor);

	//IInteractComponent
	virtual void Interact_Implementation() override;
	virtual void GetInteractComponentData_Implementation(FInteractComponentData& InteractComponentStruct) override;
	virtual void SetInteractComponentData_Implementation(FInteractComponentData InteractComponentStruct) override;

public:
	/* Components */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact | Components")
		UBoxComponent* InteractTrigger;

	/* Variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact | Components")
		FInteractComponentData InteractComponentData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interact | Components")
		FName CannotInteractTag;

	/* Interfaces */
	//IInteractable_Basic
	IIInteractable_Basic* Interactable_Basic_Interface;

private:
	ACharacter* Owner;

	TArray <UObject*> ObjectWithInterface;
};
