// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "../Interfaces/IPickComponent.h"
#include "../Interfaces/IPickableObject_Basic.h"
#include "../GeneralStructLibrary.h"
#include "Pick_Component.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class ALSV4_CPP_API UPick_Component : public UActorComponent, public IIPickComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPick_Component();

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
	 * Fuction wich recives the candidate actor to pick and gets its name to update the action notification of the HUD, if its nullptr means there is no actor
	 * @param Actor - Candidate actor to update the HUD
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pick | Functions")
		void UpdateHUDBP(AActor* Actor);

	/* Functions */
	virtual void Pick_Implementation() override;
	virtual void GetPickComponentData_Implementation(FPickComponentData& PickComponentStruct) override;
	virtual void SetPickComponentData_Implementation(FPickComponentData PickComponentStruct) override;

public:
	/* Components */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pick | Components")
		UBoxComponent* PickTrigger;

	/* Variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pick | Properties")
		FPickComponentData PickComponentData;

	/* Interfaces */
	//IPickableObject_Basic
	IIPickableObject_Basic* PickableObjecBasic_Interface;
	FPickableObjectData_Basic PickableObjectData_Basic;

private:
	ACharacter* Owner;
};
