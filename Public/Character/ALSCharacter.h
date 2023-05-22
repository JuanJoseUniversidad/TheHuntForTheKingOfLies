// Copyright:       Copyright (C) 2022 Doğa Can Yanıkoğlu
// Source Code:     https://github.com/dyanikoglu/ALS-Community


#pragma once

#include "CoreMinimal.h"
#include "Character/ALSBaseCharacter.h"

#include "../Interfaces/IEquipableObject_Basic.h"
#include "../Interfaces/IEquipableObject_Reload.h"
#include "../Interfaces/IEquipSystem.h"
#include "../Interfaces/IAimSystem.h"
#include "../Interfaces/IUseSystem.h"
#include "../Interfaces/IReloadSystem.h"
#include "../Interfaces/ICraftKnowledgeSystem.h"

#include "../Components/Dash_Component.h"
#include "../Components/Consume_Component.h"
#include "../Components/Inventory_Component.h"
#include "../Components/Pick_Component.h"
#include "../Components/Interact_Component.h"
#include "../Components/ALSHealth_Component.h"

#include "../ShotgunAmmo.h"



#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "Sound/SoundCue.h"
#include "Math/Vector.h"
#include "Components/SkinnedMeshComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Camera/CameraShakeBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/AnimMontage.h"
#include "Components/SphereComponent.h"
#include "ALSCharacter.generated.h"

class UAIPerceptionStimuliSourceComponent;

/**
 * Specialized character class, with additional features like held object etc.
 */
UCLASS(Blueprintable, BlueprintType)
class ALSV4_CPP_API AALSCharacter : public AALSBaseCharacter, public IIEquipSystem, public IIAimSystem, public IIUseSystem, public IIReloadSystem, public IICraftKnowledgeSystem
{
	GENERATED_BODY()

public:
	AALSCharacter(const FObjectInitializer& ObjectInitializer);

	/** Implemented on BP to update held objects */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALS|HeldObject")
	void UpdateHeldObject();

	UFUNCTION(BlueprintCallable, Category = "ALS|HeldObject")
	void ClearHeldObject();

	UFUNCTION(BlueprintCallable, Category = "ALS|HeldObject")
	void AttachToHand(UStaticMesh* NewStaticMesh, USkeletalMesh* NewSkeletalMesh,
	                  class UClass* NewAnimClass, bool bLeftHand, FVector Offset);

	virtual void RagdollStart() override;

	virtual void RagdollEnd() override;

	virtual ECollisionChannel GetThirdPersonTraceParams(FVector& TraceOrigin, float& TraceRadius) override;

	virtual FTransform GetThirdPersonPivotTarget() override;

	virtual FVector GetFirstPersonCameraTarget() override;



protected:
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	virtual void OnOverlayStateChanged(EALSOverlayState PreviousState) override;

	//EDITED
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;

	/** Implement on BP to update animation states of held objects */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALS|HeldObject")
	void UpdateHeldObjectAnimations();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|Component")
	TObjectPtr<USceneComponent> HeldObjectRoot = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|Component")
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|Component")
	TObjectPtr<UStaticMeshComponent> StaticMesh = nullptr;


	//--== !! Not ALS native from here !! ==--//

	/*-= ADDITIONAL FUNCTIONS =-*/
public:
	/**
	 * Check if can perform an action by a given parameter
	 * @return The bool value if can perform the action
	 * @param Action - The action wich want perform
	 */
	bool CanPerformAction(EALSAction Action);

	/**
	 * Reset the original Gait when an action finalize like heal or reload
	 */
	void ResetOriginalGait();

	UFUNCTION(BlueprintCallable, Category = "ALS | Character")
	void SetVisibleActorVisualsOnly(bool bNewVisibility);

	void SetEquippableObjectVisible(bool bNewVisibility);

public:
	/*-= ADDITIONAL COMPONENTS =-*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|Equip|Component")
		UChildActorComponent* EquippedActorComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ALS|AI|Component")
		UAIPerceptionStimuliSourceComponent* StimuliSourceComp;

public:
	/*-= ADDITIONAL PROPERTIES =-*/
	UPROPERTY(EditAnywhere, Category = "ALS | Essential Information")
		EALSOverlayState DefaultState = EALSOverlayState::Default;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS | Ragdoll")
		float GetUpFromRagdollThreshold = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS | Ragdoll")
		FName BoneNameRagdollVelocity = "Root";

	/*-= INTERFACES VARIABLE =-*/
public:
	//Equippable object basic
	IIEquipableObject_Basic* EquippableObjectBasic_Interface;
	FEquipableObjectData_Basic EquipableObjectDataBase;

	//Equippable object reload
	IIEquipableObject_Reload* EquippableObjectReload_Interface;
	FEquipableObjectData_Reload EquippableObjectDataReload;


	/*-= EQUIP SYSTEM =-*/
#pragma region EquipSystem

	/* Functions */
public:
	virtual void Equip_Implementation() override;
	virtual void Unequip_Implementation() override;
	virtual void GetEquipSystemData_Implementation(FEquipSystemData& EquipSystemStruct) override;
	virtual void SetEquipSystemData_Implementation(FEquipSystemData EquipSystemStruct) override;

	/**
	 * Fuction wich recives the input of the player and execute the equip or unequip actions
	 * @see CharacterController.cpp on the function EquipAction
	 * @param bValue - If the input its pressed or released
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS | Equip")
		void EquipAction(bool bValue);

	/**
	 * Fuction wich recives the input of the player when equip and execute a Blueprint layer
	 * @param bValue - If the input its pressed or released
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Reload")
		void EquipBP(bool bValue);

private:
	/**
	 * Set all the references of the new equippable object if its passed, in other case set the references of the actual equipped object
	 * @param Actor - The new object wich want to be equipped to the character, nullptr means stay with the current object.
	 */
	void SetNewEquippableObject(TSubclassOf<AActor> NewEquippableObject = nullptr);

	/* Variables */
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|Equip|Component")
		USceneComponent* EquippedPivot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|Equip|Component")
		UPhysicsConstraintComponent* PhysicsContraint;

	UPROPERTY(EditAnywhere, Category = "ALS|Equip|Component")
		UStaticMeshComponent* PhysicsAnchor;

	UPROPERTY(EditAnywhere, Category = "ALS|Equip|Component")
		UStaticMeshComponent* ObjectAnchor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|Equip|Properties")
		FEquipSystemData EquipSystemData;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ALS|Equip|Properties")
		FName AffiliationTagName;
#pragma endregion 


	/*-= AIM SYSTEM =-*/
#pragma region AimSystem

	/* Functions */
public:
	virtual void Aim_Implementation(bool bAiming) override;
	virtual void GetAimSystemData_Implementation(FAimSystemData& AimSystemStruct) override;
	virtual void SetAimSystemData_Implementation(FAimSystemData AimSystemStruct) override;

	//Aim system and HUD crosshair
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS | Aim")
		void UpdateCrosshairOpacity(bool bValue);

	/* Variables */
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS | Aim | Properties")
		FAimSystemData AimSystemData;
#pragma endregion


	/*-= USE SYSTEM =-*/
#pragma region UseSystem

	/* Functions */
public:
	virtual void Use_Implementation() override;
	virtual void GetUseSystemData_Implementation(FUseSystemData& UseSystemStruct) override;
	virtual void SetUseSystemData_Implementation(FUseSystemData UseSystemStruct) override;

	/**
	 * Fuction wich recives the input of the player and execute the use action
	 * @see CharacterController.cpp on the function UseAction
	 * @param bValue - If the input its pressed or released
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Use")
		void UseAction(bool bValue);

	/**
	 * Fuction wich recives the input of the player when use and execute a Blueprint layer
	 * @param bValue - If the input its pressed or released
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Reload")
		void UseBP(bool bValue);

	/**
	 * Enables that can use the equipped object with a delay
	 * @param Delay - Time to enable the player can use the equipped object
	 */
	UFUNCTION(BlueprintCallable, Category = "ALS|Use")
		void EnableCanUse(float delay);

	/**
	 * Disables that can use the equipped object and clears the timer
	 */
	UFUNCTION(BlueprintCallable, Category = "ALS|Use")
		void DisableCanUse();

	/* Variables */
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS | Use | Properties")
		FUseSystemData UseSystemData;

private:
	FTimerHandle TimerHandleUse;
#pragma endregion


	/*-= RELOAD SYSTEM =-*/
#pragma region ReloadSystem
public:
	/* Functions */
	virtual void ReloadMode_Implementation(bool bReloadMode) override;
	virtual void Reloading_Implementation(bool bReloading, int32 TypeOfProjectile) override;
	virtual void GetReloadSystemData_Implementation(FReloadSystemData& ReloadSystemStruct) override;
	virtual void SetReloadSystemData_Implementation(FReloadSystemData ReloadSystemStruct) override;

	/**
	 * Fuction wich recives the input of the player and execute the reload action
	 * @see CharacterController.cpp on the function ReloadAction
	 * @param bValue - If the input its pressed or released
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Reload")
		void EnterReloadModeAction(bool bValue);

	/**
	 * Fuction wich recives the input of the player when enter on reload mode and execute a Blueprint layer
	 * @param bValue - If the input its pressed or released
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Reload")
		void EnterReloadModeBP(bool bValue);

public:
	/* Variables */
	UPROPERTY(EditAnywhere, Category = "ALS | Reload | Component")
		UStaticMeshComponent* ReloadShellAnchor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS | Reload | Properties")
		FReloadSystemData ReloadSystemData;

	bool bWantToSprint = false; //When ends of reload or other action an still with the sprint key, after exit the reload mode start to sprit
	bool bWantToWalk = false; //When ends of reload or other action an still with the walk key, after exit the reload mode start to walk
	bool bWantToReload = false; //When ends of reload a projectile and the reload mode key its not pressed then when finish to reload the projectile exit on this mode, else stills in this mode

private:
	FTimerHandle TimerHandleReloadMode;
#pragma endregion


	/*-= CRAFT KNOWLEDGE SYSTEM =-*/
#pragma region CraftKnowledgeSystem

		/* Functions */
public:
	virtual void Learn_Implementation(FCrafteableObjectData Recipe) override;
	virtual void Forget_Implementation(FCrafteableObjectData Recipe) override;
	virtual void GetCraftKnowledgeSystemData_Implementation(FCraftKnowledgeSystemData& CraftKnowledgeSystemStruct) override;
	virtual void SetCraftKnowledgeSystemData_Implementation(FCraftKnowledgeSystemData CraftKnowledgeSystemStruct) override;

	/* Variables */
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS | Craft | Properties")
		FCraftKnowledgeSystemData CraftKnowledgeSystemData;
#pragma endregion





	/*-= DASH COMPONENT =-*/
#pragma region DashComponent
public:
	/* Functions */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Dash")
		void DashAction(bool bValue);

public:
	/* Variables */
	UDash_Component* DashComponent;
#pragma endregion


	/*-= CONSUME COMPONENT =-*/
#pragma region ConsumeComponent
public:
	/* Functions */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Consume")
		void ConsumeAction(bool bValue);

public:
	/* Variables */
	UConsume_Component* ConsumeComponent;
#pragma endregion


	/*-= INVENTORY COMPONENT =-*/
#pragma region InventoryComponent
public:
	/* Functions */
public:
	/* Variables */
	UInventory_Component* InventoryComponent;
#pragma endregion


	/*-= PICK COMPONENT =-*/
#pragma region PickComponent
public:
	/* Functions */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Pick")
		void PickAction(bool bValue);
public:
	/* Variables */
	UPick_Component* PickComponent;
#pragma endregion


	/*-= INTERACT COMPONENT =-*/
#pragma region InteractComponent
public:
	/* Functions */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Pick")
		void InteractAction(bool bValue);
public:
	/* Variables */
	UInteract_Component* InteractComponent;
#pragma endregion

	/*-= HEALTH COMPONENT =-*/
#pragma region InventoryComponent
public:
	/* Functions */
public:
	/* Variables */
	UALSHealth_Component* ALSHealthComponent;
#pragma endregion



private:
	bool bNeedsColorReset = false;
};
