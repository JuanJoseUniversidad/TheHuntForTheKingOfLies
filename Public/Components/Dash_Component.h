// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Interfaces/IDashComponent.h"
#include "../GeneralStructLibrary.h"

#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

#include "Sound/SoundCue.h"
#include "Camera/CameraShakeBase.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Components/CapsuleComponent.h"

#include "Math/UnrealMathUtility.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "GenericPlatform/GenericPlatformProcess.h"
#include "Containers/Array.h"

#include "../Library/ALSCharacterEnumLibrary.h"

#include "Dash_Component.generated.h"

class UNiagaraComponent;
class UAnimMontage;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALSV4_CPP_API UDash_Component : public UActorComponent, public IIDashComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDash_Component();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool GetMakeVisibleCharacter();

	/* Functions */
	virtual void StartDash_Implementation(bool IsCrouch, EALSEightDirection AlternativeDirection = EALSEightDirection::None) override;
	virtual void EndDash_Implementation(float ReuseDashDelay, float EnableAnimationsDelay, float VisiblePlayerDelay) override;
	virtual void GetDashComponentData_Implementation(FDashComponentData& DashComponentStruct) override;
	virtual void SetDashComponentData_Implementation(FDashComponentData DashComponentStruct) override;

public:
	/* Components */
	/**UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash | Components")
		UNiagaraComponent* DashEffectComponent;*/

	/* Variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash | Properties")
		FDashComponentData DashComponentData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash | Properties | FX")
		UNiagaraSystem* DashEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash | Properties | FX")
		FVector DashEffectLocationOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash | Properties | FX")
		FVector DashEffectScaleOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash | Properties | FX")
		UNiagaraSystem* DashAvalibleEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash | Properties | FX")
		bool bEffectDashAttached = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash | Properties | Sound")
		USoundCue* DashSoundCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash | Properties | Sound")
		USoundCue* DashAvalibleSoundCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash | Properties | Feedback")
		UForceFeedbackEffect* DashFeedbackForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash | Properties | Feedback")
		TSubclassOf<UCameraShakeBase> DashCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash | Properties | Feedback")
		float ScaleDashCamShake = 1.0f;

	float VelocityZPreDash;

protected:
	/* Makes the character visible while its dashing */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash | Properties")
		bool bMakeVisibleCharacter;

	/*Map of animations based on the direction given, the vector must contain only 1 or 0 as values*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash | Properties")
		TMap<EALSEightDirection, UAnimMontage*> DirectionAnimations;


private:
	//To detect the direction of the dash
	float ForwardValue;
	float RightValue;

	float DashDirectionThreshold = 80.f;

	ACharacter* Owner;

	USkeletalMeshComponent* SkeletalMeshToAffect;

	FTimerHandle TimerHandleDash;
	FTimerHandle TimerHandleDashAnim;
	FTimerHandle TimerHandleDashEffect;
	FTimerHandle TimerHandleDashVisible;
	
};
