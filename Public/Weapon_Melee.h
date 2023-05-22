// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IEquipableObject_Basic.h"
#include "GeneralStructLibrary.h"
#include "Weapon_Melee.generated.h"

class IIEquipableObject_Basic;
class USoundCue;
class USceneComponent;
struct FEquipableObjectData_Basic;
struct FCollisionQueryParams;
struct FALSBallisticHitFX;

UCLASS()
class ALSV4_CPP_API AWeapon_Melee : public AActor, public IIEquipableObject_Basic
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon_Melee();

	//IIEquipableObject_Basic
	virtual float TryToUse_Implementation(int32 UseMode = 0) override;
	virtual bool Use_Implementation() override;
	virtual void SetVisibleMesh_Implementation(bool Visible) override;
	virtual void PlayReadySound_Implementation() override;
	virtual void StopReadySound_Implementation() override;
	virtual void GetEquipableObjectData_Basic_Implementation(FEquipableObjectData_Basic& EquipableObjectStruct) override;
	virtual void SetEquipableObjectData_Basic_Implementation(FEquipableObjectData_Basic EquipableObjectStruct) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USceneComponent* SceneComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* WeaponMesh;

	//Properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties | BaseBehaviour")
		FEquipableObjectData_Basic EquipableObjectData_Basic;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Properties | BaseBehaviour")
		FName InitBoxSocketName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Properties | BaseBehaviour")
		FName EndBoxSocketName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Properties | BaseBehaviour")
		FName SectionMontageUsageName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties | BaseBehaviour")
		float TraceRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties | BaseBehaviour")
		float TimeForNextTrace;

	UPROPERTY(EditAnywhere, Category = "Properties | Data")
		TEnumAsByte<ETraceTypeQuery> TraceChannel;

	UPROPERTY(EditAnywhere, Category = "Properties | Data")
		float Damage = 5.0f;

	//The impulse that the bones of the body will recive
	UPROPERTY(EditAnywhere, Category = "Properties | Data")
		double BoneImpulse = 5000.0;

	//The impulse per hit that the integrity of the character will be recive
	UPROPERTY(EditAnywhere, Category = "Properties | Data")
		double KnockbackImpulse = 5000.0;

	UPROPERTY(EditAnywhere, Category = "Properties | Data")
		TObjectPtr<UDataTable> ImpactFXDataTable;

	UPROPERTY(EditAnywhere, Category = "Properties | Sound")
		USoundCue* UseSoundCue;

	UPROPERTY(EditAnywhere, Category = "Properties | Sound")
		USoundCue* AimSoundCue;

private:
	bool bCanPlayAimSound;
	bool bIsUsing;

	float SectionUsageDuration;

	FTimerHandle TimerHandleAimSound;
	FTimerHandle TimerHandleUse;
	FTimerHandle TimerHandleResetUse;
	FCollisionQueryParams TraceQueryParams;

	TArray<FALSBallisticHitFX*> ImpactFXArray;
	TArray<AActor*>ActorsToIgnore;
};
