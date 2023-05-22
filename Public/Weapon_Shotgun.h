// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

#include "Interfaces/IEquipableObject_Basic.h"
#include "Interfaces/IEquipableObject_Reload.h"
#include "Interfaces/IProjectile_Basic.h"

#include "GeneralStructLibrary.h"

#include "UObject/Class.h"

#include "ShotgunAmmo.h"

#include "Animation/AnimMontage.h"

#include "Math/UnrealMathUtility.h"

#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "GameFramework/ForceFeedbackEffect.h"

#include "../Public/Character/ALSBaseCharacter.h"
#include "Weapon_Shotgun.generated.h"

UCLASS()
class ALSV4_CPP_API AWeapon_Shotgun : public AActor, public IIEquipableObject_Basic, public IIEquipableObject_Reload
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon_Shotgun();

	//IIEquipableObject_Basic
	virtual float TryToUse_Implementation(int32 UseMode = 0) override;
	virtual bool Use_Implementation() override;
	virtual void SetVisibleMesh_Implementation(bool Visible) override;
	virtual void PlayReadySound_Implementation() override;
	virtual void StopReadySound_Implementation() override;
	virtual void GetEquipableObjectData_Basic_Implementation(FEquipableObjectData_Basic& EquipableObjectStruct) override;
	virtual void SetEquipableObjectData_Basic_Implementation(FEquipableObjectData_Basic EquipableObjectStruct) override;

	//IIEquipableObject_Reload
	virtual bool Reload_Implementation() override;
	virtual bool CheckStatus_Implementation() override;
	virtual TSubclassOf<AActor> SetReadyLoadType_Implementation(int32 Slot) override;
	virtual void SetReadyLoad_Implementation() override;
	virtual void SetLoadsTypesToReload_Implementation(int32 Slot, TSubclassOf<AActor> Type) override;
	virtual void GetEquipableObjectData_Reload_Implementation(FEquipableObjectData_Reload& EquipableObjectStruct) override;
	virtual void SetEquipableObjectData_Reload_Implementation(FEquipableObjectData_Reload EquipableObjectStruct) override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	//Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		USceneComponent* SceneComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		USkeletalMeshComponent* ShotgunMesh;


	//Properties
	UPROPERTY(EditAnywhere, Category = "Properties | BaseBehaviour")
		FEquipableObjectData_Basic EquipableObjectData_Basic;

	UPROPERTY(EditAnywhere, Category = "Properties | BaseBehaviour")
		FEquipableObjectData_Reload EquipableObjectData_Reload;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties | AdditionalProperties")
		int32 MagazineSize = 8;

	UPROPERTY(EditAnywhere, Category = "Properties | AdditionalProperties")
		float Recoil = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Properties | AdditionalProperties")
		FName SocketMuzzleName = "Muzzle";

	UPROPERTY(EditAnywhere, Category = "Properties | AdditionalProperties")
		FName SocketShellEjectName = "ShellEject";

	UPROPERTY(EditAnywhere, Category = "Properties | AdditionalProperties")
		FName NiagaraParamenterShellName = "ShellMesh";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties | AdditionalProperties", meta = (MustImplement = "IProjectile_Basic"))
		TArray<TSubclassOf<AActor>> Magazine;

	UPROPERTY(EditAnywhere, Category = "Properties | Animation")
		UAnimMontage* GunMontage;

	UPROPERTY(EditAnywhere, Category = "Properties | Sound")
		USoundCue* AimSoundCue;

	UPROPERTY(EditAnywhere, Category = "Properties | Sound")
		USoundCue* UseSoundCue;

	UPROPERTY(EditAnywhere, Category = "Properties | Sound")
		USoundCue* DryUseSoundCue;

	UPROPERTY(EditAnywhere, Category = "Properties | FX")
		UNiagaraSystem* MuzzleEffect;

	UPROPERTY(EditAnywhere, Category = "Properties | FX")
		UNiagaraSystem* ShellEjectEffect;

	UPROPERTY(EditAnywhere, Category = "Properties | Feedback")
		UForceFeedbackEffect* ShootFeedbackForce;

	UPROPERTY(EditAnywhere, Category = "Properties | Feedback")
		TSubclassOf<UCameraShakeBase> CameraShake;

	FProjectileData ProjectileData;

private:
	void LoadGunChamber();

private:
	bool bCanPlayAimSound = true;
	FTimerHandle TimerHandleAimSound;
	IIProjectile_Basic* ProyectileBasic_Interface;
};
