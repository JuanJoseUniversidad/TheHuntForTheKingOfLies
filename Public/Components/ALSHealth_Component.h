// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"

#include "../GeneralStructLibrary.h"
#include "../Interfaces/IHealthComponent.h"

#include "PhysicsEngine/PhysicalAnimationComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Animation/AnimMontage.h"

#include "Library/ALSCharacterEnumLibrary.h"
#include "Library/ALSCharacterStructLibrary.h"
#include "Engine/DataTable.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#include "../Character/ALSBaseCharacter.h"

#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "Components/DecalComponent.h"
#include "Components/PostProcessComponent.h"
#include "Components/CapsuleComponent.h"


#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

#include "Camera/CameraShakeBase.h"

#include "ALSHealth_Component.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class ALSV4_CPP_API UALSHealth_Component : public UActorComponent, public IIHealthComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UALSHealth_Component();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/**
	 * Calculate the probability of resist a Knockout based on the knockback force and its tolerance
	 * If the excess of substracting the force and the tolerance its bigger the the probabilities are bigger
	 * There probability of resist the knockout its 20% as base, from there apply the excess of the force
	 * @param KnockbackForce - Knockback force of the impact
	 * @param KnockbackTolerance - Tolerance to the knockbacks forces
	 * @return If its true the resist the knockback if not then returns false
	 */
	bool CanResistKnockback(float KnockbackForce, float KnockbackTolerance);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//IHealthComponent
	virtual void AddHealth_Implementation(float Life) override;
	virtual bool Revive_Implementation() override;
	virtual void HitDamage_Implementation(float Damage, const FHitResult& Hit, double BoneImpulse = 5000.0, double KnockbackImpulse = 5000.0) override;
	virtual void FallDamage_Implementation(float Damage, const FHitResult& Hit) override;
	virtual void GetHealthComponentData_Implementation(FHealthComponentData& HealthComponentStruct) override;
	virtual void SetHealthComponentData_Implementation(FHealthComponentData HealthComponentStruct) override;

	/*
	 * Retrive life
	 * @param Damage - The damage to subtract to the life
	 */
	UFUNCTION(BlueprintCallable, Category = "Health")
		void AddDamage(float Damage);

	/*
	 * Event hit triggered when its on ragdoll mode and hit any surface
	 * @param HitComponent - The hitted component
	 * @param OtherActor - The actor responsible for the hit
	 * @param OtherComp - The component responsible for the hit
	 * @param NormalImpulse - The normal impulse of the hit
	 * @param Hit - The hit result
	 */
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	/*
	 * Sets all the parameters to disable the owner actor 
	 */
	UFUNCTION(BlueprintCallable, Category = "Health")
		void Dead();

	/*
	 * Creates a widget when the owner dies
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Health")
		void CreateWidget();

public:
	/* Variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health | Properties")
		FHealthComponentData HealthComponentData;

#pragma region Postprocess
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health | Properties |Postprocess")
		FPostProcessSettings HurtPostprocessData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health | Properties |Postprocess")
		FPostProcessSettings DeathPostprocessData;
#pragma endregion

#pragma region Death

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health | Properties | Death")
		bool bCreateWidget = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health | Properties | Death")
		bool bActivateSlowMotionOnDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health | Properties | Death")
		float DeathSlowMotion = 0.15f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health | Properties | Death")
		bool bFadeAudioDeath = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health | Properties | Death")
		float FadeSoundVolume = 0.15f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health | Properties | Death")
		float FadeTime = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health | Properties | Death")
		USoundClass* FadeSoundClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health | Properties | Death")
		USoundMix* FadeSoundMixer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health | Properties | Death")
		USoundCue* DeathEffectSoundCue;
#pragma endregion

#pragma region Damage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health | Component | Damage")
		UPostProcessComponent* PostProcessHurted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health | Component | Death")
		UPostProcessComponent* PostProcessDead;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health | Properties | Damage")
		bool bActivatePostProcess = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health | Properties | Damage")
		float DelayFadeOutPostProcessEffect = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health | Properties | Damage")
		float FadeOutTimePostProcess = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health | Properties | Damage")
		bool bActivateFeedbackController = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health | Properties | Damage")
		bool bActivateCamerashakeController = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health | Properties | Damage", meta = (Units = "Percent", UIMin = 0, UIMax = 100))
		float HealthPostProcessThreshold = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health | Properties | Damage")
		USoundCue* PainSoundCue;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health | Properties | Hit damage")
		float ScaleHitImpactCamShake = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health | Properties | Hit damage")
		UForceFeedbackEffect* ImpactHitFeedbackForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health | Properties | Hit damage")
		TSubclassOf<UCameraShakeBase> HitImpactCameraShake;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health | Properties | Fall damage")
		TSubclassOf<UCameraShakeBase> FallImpactCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health | Properties | Fall damage")
		float ScaleImpactCamShake = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health | Properties | Fall damage")
		UForceFeedbackEffect* ImpactFallingFeedbackForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health | Properties | Fall damage")
		TObjectPtr<UDataTable> FallingImpactDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health | Properties | Fall damage | Decal")
		TArray<TSoftObjectPtr<UMaterialInterface>> BloodSplatDecalsMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health | Properties | Fall damage | Decal")
		FVector DecalLocationOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health | Properties | Fall damage | Decal")
		FRotator DecalRotationOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health | Properties | Fall damage | Decal")
		FVector DecalSize = FVector(35, 35, 35);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health | Properties | Fall damage | Decal")
		float DecalLifeSpan = 6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health | Properties | Fall damage | Decal")
		float DecalFadeOut = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health | Properties | Fall damage | FX")
		TSoftObjectPtr<UNiagaraSystem> BloodSplatParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health | Properties | Fall damage | FX")
		FVector BloodSplatParticleLocationOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health | Properties | Fall damage | FX")
		FRotator BloodSplatParticleRotationOffset;
#pragma endregion

private:
	FTimerHandle TimerHandleFallRagdollRestart;
	FTimerHandle TimerHandleEnablePostProccessFade;
	FTimerHandle TimerHandleDestroyActor;
	FTimerHandle TimerHandleEndAccumulateKnockback;

	//Owner reference
	AALSBaseCharacter* Owner;

	/*Ragdoll falling system*/
	bool bIsHittedFall = false;
	bool bFadeOutPostProcess = false;
	TArray<FALSFallImpactFX*> FallImpactFXArray;

	/*Hit impact system procelular animation*/
	float RecoveryStatus;

	//The accumulated knockback from a period of time
	float AccumulatedKnockback;
};
