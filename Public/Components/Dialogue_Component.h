// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Interfaces/IInteractable_Basic.h"
#include "../Interfaces/IDialogueComponent.h"
#include "../GeneralStructLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/OutputDeviceNull.h"
#include "Dialogue_Component.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class ALSV4_CPP_API UDialogue_Component : public UActorComponent, public IIInteractable_Basic, public IIDialogueComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDialogue_Component();

	//IInteractable_Basic
	virtual bool Interact_Implementation(AActor* Interactor = nullptr) override;
	virtual bool EndInteract_Implementation() override;
	virtual void GetInteractableData_Basic_Implementation(FInteractableData_Basic& InteractableStruct) override;
	virtual void SetInteractableData_Basic_Implementation(FInteractableData_Basic InteractableStruct) override;

	//IDialogueComponent
	virtual void ConstructFullName_Implementation(FName Name = "", FName Nickname = "") override;
	virtual FName GetFullName_Implementation() override;
	virtual void SetInteractor_Implementation(AActor* Interactor) override;
	virtual AActor* GetInteractor_Implementation() override;
	virtual void GetDialogueComponentData_Implementation(FDialogueComponentData& DialogueComponentStruct) override;
	virtual void SetDialogueComponentData_Implementation(FDialogueComponentData DialogueComponentStruct) override;
public:
	/**
	 * Function which creates the craft station interface with a blueprint layer
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Dialogue | Functions")
		void CreateDialogInterface();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	//Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		FInteractableData_Basic Interactable_Basic;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		FDialogueComponentData DialogueComponentData;

	//Reference to the interface displayed when the dialog is triggered
	UPROPERTY(BlueprintReadWrite, Category = "Properties")
		UObject* InterfaceReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		FString DialogCloseFunctionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", AdvancedDisplay)
		float DialogDistance = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", AdvancedDisplay)
		float DialogueOffset = -40.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		float DialogueThresholfForOffset = 15.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Properties")
		FName CannotFocusTag;

private:
	//Properties
		//Movement mode before enter on dialog mode
	EMovementMode OwnerMovementMode;
	EMovementMode InteractorMovementMode;

	ACharacter* CharacterOwner;
	ACharacter* CharacterInteractor;


	FRotator InterpolatedRotation;
	bool bInterpolationCameraComplete = false;
	float OffsetCameraRotator = 0.0f;
};
