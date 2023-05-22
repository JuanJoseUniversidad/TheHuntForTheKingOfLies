// Copyright:       Copyright (C) 2022 Doğa Can Yanıkoğlu
// Source Code:     https://github.com/dyanikoglu/ALS-Community

// Modified by: Juan José Gómez Simón

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "ALSPlayerController.generated.h"

class AALSBaseCharacter;
class AALSCharacter;
class IIAimSystem;
class IIReloadSystem;
class UInputMappingContext;

/**
 * Player controller class
 */
UCLASS(Blueprintable, BlueprintType)
class ALSV4_CPP_API AALSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void OnPossess(APawn* NewPawn) override;

	virtual void OnRep_Pawn() override;

	virtual void SetupInputComponent() override;

	virtual void BindActions(UInputMappingContext* Context);

	/* EDITED */
	/**
	 * Fuction wich recives the input of the player when enter on pause and execute a Blueprint layer
	 * @param bValue - If the input its pressed or released
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Pause")
		void SetPauseBP(bool bValue);

protected:
	void SetupInputs();

	void SetupCamera();

	UFUNCTION()
	void ForwardMovementAction(const FInputActionValue& Value);

	UFUNCTION()
	void RightMovementAction(const FInputActionValue& Value);

	UFUNCTION()
	void CameraUpAction(const FInputActionValue& Value);

	UFUNCTION()
	void CameraRightAction(const FInputActionValue& Value);

	UFUNCTION()
	void JumpAction(const FInputActionValue& Value);

	UFUNCTION()
	void EquipAction(const FInputActionValue& Value);//--

	UFUNCTION()
	void UseAction(const FInputActionValue& Value);//--

	UFUNCTION()
	void ReloadModeAction(const FInputActionValue& Value);//--

	UFUNCTION()
	void ReloadFirstAmmoAction(const FInputActionValue& Value);//--

	UFUNCTION()
	void ReloadSecondAmmoAction(const FInputActionValue& Value);//--

	UFUNCTION()
	void ReloadThirdAmmoAction(const FInputActionValue& Value);//--

	UFUNCTION()
	void DashAction(const FInputActionValue& Value);//--

	UFUNCTION()
	void ConsumeAction(const FInputActionValue& Value);//--

	UFUNCTION()
	void PickAction(const FInputActionValue& Value);//--

	UFUNCTION()
	void InteractAction(const FInputActionValue& Value);//--

	UFUNCTION()
	void PauseAction(const FInputActionValue& Value);//--

	UFUNCTION()
	void SprintAction(const FInputActionValue& Value);

	UFUNCTION()
	void AimAction(const FInputActionValue& Value);

	UFUNCTION()
	void CameraTapAction(const FInputActionValue& Value);

	UFUNCTION()
	void CameraHeldAction(const FInputActionValue& Value);

	UFUNCTION()
	void StanceAction(const FInputActionValue& Value);

	UFUNCTION()
	void WalkAction(const FInputActionValue& Value);

	UFUNCTION()
	void RagdollAction(const FInputActionValue& Value);

	UFUNCTION()
	void VelocityDirectionAction(const FInputActionValue& Value);

	UFUNCTION()
	void LookingDirectionAction(const FInputActionValue& Value);

	// Debug actions
	UFUNCTION()
	void DebugToggleHudAction(const FInputActionValue& Value);

	UFUNCTION()
	void DebugToggleDebugViewAction(const FInputActionValue& Value);

	UFUNCTION()
	void DebugToggleTracesAction(const FInputActionValue& Value);

	UFUNCTION()
	void DebugToggleShapesAction(const FInputActionValue& Value);

	UFUNCTION()
	void DebugToggleLayerColorsAction(const FInputActionValue& Value);

	UFUNCTION()
	void DebugToggleCharacterInfoAction(const FInputActionValue& Value);

	UFUNCTION()
	void DebugToggleSlomoAction(const FInputActionValue& Value);

	UFUNCTION()
	void DebugFocusedCharacterCycleAction(const FInputActionValue& Value);

	UFUNCTION()
	void DebugToggleMeshAction(const FInputActionValue& Value);

	UFUNCTION()
	void DebugOpenOverlayMenuAction(const FInputActionValue& Value);

	UFUNCTION()
	void DebugOverlayMenuCycleAction(const FInputActionValue& Value);

	//EDIT
	//Other functions
	UFUNCTION(BlueprintCallable, Category = "ALS|Pause")
		void SimulateAllActionsKeys(float Value = 0.0f);

public:
	/** Main character reference */
	UPROPERTY(BlueprintReadOnly, Category = "ALS")
	TObjectPtr<AALSBaseCharacter> PossessedCharacter = nullptr;

	/** Character inherith*/
	UPROPERTY(BlueprintReadOnly, Category = "ALS")
	TObjectPtr<AALSCharacter> PossessedPlayerCharacter = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ALS|Input")
	TObjectPtr<UInputMappingContext> DefaultInputMappingContext = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ALS|Input")
	TObjectPtr<UInputMappingContext> DebugInputMappingContext = nullptr;

	//-- Edited --//
	IIAimSystem* AimSystem;
	IIReloadSystem* ReloadSystem;
};
