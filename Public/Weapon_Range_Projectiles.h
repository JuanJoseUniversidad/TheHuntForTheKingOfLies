// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IEquipableObject_Basic.h"
#include "Projectile.h"
#include "Weapon_Range_Projectiles.generated.h"

class IIEquipableObject_Basic;
class UAnimMontage;
class USoundCue;
class USceneComponent;
class UNiagaraSystem;
class AProjectile;
struct FEquipableObjectData_Basic;


UCLASS(Blueprintable, BlueprintType)
class ALSV4_CPP_API AWeapon_Range_Projectiles : public AActor, public IIEquipableObject_Basic
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon_Range_Projectiles();

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

	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USkeletalMeshComponent* WeaponSkeletalMesh;


	//Properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties | BaseBehaviour")
		FEquipableObjectData_Basic EquipableObjectData_Basic;

	UPROPERTY(EditAnywhere, Category = "Properties | BaseBehaviour")
		float Dispersion;

	UPROPERTY(EditAnywhere, Category = "Properties | BaseBehaviour")
		TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Properties | BaseBehaviour")
		FName MuzzleSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties | BaseBehaviour")
		UAnimMontage* WeaponAnimation;

	UPROPERTY(EditAnywhere, Category = "Properties | Sound")
		USoundCue* UseSoundCue;

	UPROPERTY(EditAnywhere, Category = "Properties | Sound")
		USoundCue* AimSoundCue;

	UPROPERTY(EditAnywhere, Category = "Properties | FX")
		UNiagaraSystem* MuzzleEffect;

private:
	bool bCanPlayAimSound;
	bool bIsUsing;

	float SectionUsageDuration;

	FTimerHandle TimerHandleAimSound;
	FTimerHandle TimerHandleUse;
	FTimerHandle TimerHandleResetUse;


	TArray<AActor*>ActorsToIgnore;
};
