// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ALSHealth_Component.h"

// Sets default values for this component's properties
UALSHealth_Component::UALSHealth_Component()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	PostProcessHurted = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessHurt"));



	PostProcessDead = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessDead"));

	HealthComponentData.PhysicsAnimComponent = CreateDefaultSubobject<UPhysicalAnimationComponent>(TEXT("Physical animation component"));
	
}


// Called when the game starts
void UALSHealth_Component::BeginPlay()
{
	Super::BeginPlay();

	// ...
	Owner = Cast<AALSBaseCharacter>(GetOwner());

	PostProcessHurted->AttachToComponent(Owner->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	PostProcessHurted->BlendWeight = 0.0f;
	PostProcessHurted->Settings = HurtPostprocessData;
	
	PostProcessDead->AttachToComponent(Owner->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	PostProcessDead->BlendWeight = 0.0f;
	PostProcessDead->Settings = DeathPostprocessData;

	HealthComponentData.PhysicsAnimComponent->SetSkeletalMeshComponent(Owner->GetMesh());

	Owner->GetMesh()->OnComponentHit.AddDynamic(this, &UALSHealth_Component::OnHit);
	Owner->GetMesh()->SetAllBodiesNotifyRigidBodyCollision(true);
}

// Called every frame
void UALSHealth_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	//Fade postprocess interpolation
	if (bFadeOutPostProcess == true) {
		PostProcessHurted->BlendWeight = FMath::FInterpTo(PostProcessHurted->BlendWeight, 0.0, DeltaTime, FadeOutTimePostProcess);
		if (PostProcessHurted->BlendWeight == 0.0) {
			bFadeOutPostProcess = false;
		}
	}

	//Recovery from a hit
	if (HealthComponentData.bStartRecoveryFromHit == true) {
		RecoveryStatus = FMath::FInterpTo(RecoveryStatus, 0.0, DeltaTime, HealthComponentData.RecoveryTimeInterpolation);//Bigger the time to interpolate less time takes due its not apply directly to the SetAllBodiesBelowPhysicsBlendWeight
		if (RecoveryStatus <= 0) {
			RecoveryStatus = 0;
			HealthComponentData.PhysicsAnimComponent->ApplyPhysicalAnimationProfileBelow("spine_01", "Default", false, false);
			Owner->GetMesh()->SetAllBodiesBelowSimulatePhysics("Pelvis", false, true);
			HealthComponentData.bStartRecoveryFromHit = false;
			//Disable physics
			Owner->GetMesh()->SetSimulatePhysics(false);
			Owner->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		else {
			Owner->GetMesh()->SetAllBodiesBelowPhysicsBlendWeight("Pelvis", RecoveryStatus, false, true);
		}
	}
}

void UALSHealth_Component::AddHealth_Implementation(float Life) {
	HealthComponentData.Life += Life;

	if (HealthComponentData.Life > HealthComponentData.MaxLife) {
		HealthComponentData.Life = HealthComponentData.MaxLife;
	}

	//Activates fade of the damage post process effect
	if (HealthComponentData.Life > (HealthComponentData.MaxLife * (HealthPostProcessThreshold/100.f)) && bActivatePostProcess == true && !Owner->GetWorld()->GetTimerManager().IsTimerActive(TimerHandleEnablePostProccessFade)) {
		Owner->GetWorld()->GetTimerManager().ClearTimer(TimerHandleEnablePostProccessFade);
		Owner->GetWorld()->GetTimerManager().SetTimer(TimerHandleEnablePostProccessFade, [&]() {
			bFadeOutPostProcess = true;
			GetWorld()->GetTimerManager().ClearTimer(TimerHandleEnablePostProccessFade);
			}, DelayFadeOutPostProcessEffect, false);
	}
}

bool UALSHealth_Component::Revive_Implementation() {
	//Reset life
	HealthComponentData.bIsDead = false;
	HealthComponentData.Life = HealthComponentData.MaxLife;

	//Reenable the input
	Owner->GetWorld()->GetFirstPlayerController()->EnableInput(Owner->GetWorld()->GetFirstPlayerController());

	//Disable post process
	PostProcessHurted->BlendWeight = 0.0f;
	PostProcessDead->BlendWeight = 0.0f;

	//Getup ragdoll
	Owner->ReplicatedRagdollEnd();

	//Restars the slow motion effect
	if (UKismetSystemLibrary::IsStandalone(this) && bActivateSlowMotionOnDead == true) {
		UGameplayStatics::SetGlobalTimeDilation(this, 1.0f);
	}

	return true;
}

void UALSHealth_Component::HitDamage_Implementation(float Damage, const FHitResult& Hit, double BoneImpulse, double KnockbackImpulse) {
	if (HealthComponentData.bIsInvulnerable == false) {
		AddDamage(Damage);

		//Spawn sound pain character
		if (PainSoundCue && HealthComponentData.bIsDead == false) {
			UGameplayStatics::SpawnSoundAttached(PainSoundCue, Owner->GetMesh());
		}

		FName Bone = Hit.BoneName;
		if (Hit.BoneName == "Pelvis") {
			Bone = "spine_01";
		}

		//Play camera shake, check if the controller is not null to avoid crashes
		if (bActivateCamerashakeController == true && HealthComponentData.bIsDead == false && HitImpactCameraShake && Cast<APlayerController>(Owner->GetController())) {
			Cast<APlayerController>(Owner->GetController())->ClientStartCameraShake(HitImpactCameraShake, ScaleHitImpactCamShake);
		}

		//Execute the feedback force
		if (bActivateFeedbackController == true && HealthComponentData.bIsDead == false && ImpactHitFeedbackForce) {
			UGameplayStatics::GetPlayerController(Owner->GetWorld(), 0)->ClientPlayForceFeedback(ImpactHitFeedbackForce);
		}

		//Physics animation
		float ImpulseBone = BoneImpulse;
		float ImpulseKnockback = KnockbackImpulse;
		
		if (HealthComponentData.bIsDead == false && Owner->GetMovementState() != EALSMovementState::Ragdoll) {
			HealthComponentData.bStartRecoveryFromHit = true;
			RecoveryStatus = HealthComponentData.AnimationWeight;
			//Physics animation on hit bone
			Owner->GetMesh()->SetSimulatePhysics(true);
			Owner->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

			HealthComponentData.PhysicsAnimComponent->ApplyPhysicalAnimationProfileBelow("spine_01", "Hit", false, false);
			Owner->GetMesh()->SetAllBodiesBelowSimulatePhysics("spine_01", true, false);
		}
		else {
			//If its on ragdoll mode reduce the impulse of the bone
			ImpulseBone /= HealthComponentData.ImpulseBoneHitDivider;
			ImpulseKnockback /= HealthComponentData.ImpulseBoneHitDivider;
		}
		
		//Gets the initial location before apply the knockback for calculate the distance on an instant
		FVector InitialLocation = Owner->GetActorLocation().GetAbs();

		//Calculate direction of the impulse
		FVector Impulse = UKismetMathLibrary::Subtract_VectorVector(Hit.TraceEnd, Hit.TraceStart);
		UKismetMathLibrary::Vector_Normalize(Impulse);

		//Calculate the impulse for the bones
		FVector TotalImpluseBone = UKismetMathLibrary::Multiply_VectorFloat(Impulse, ImpulseBone);
		//Apply the impulse to the bones
		Owner->GetMesh()->AddImpulse(TotalImpluseBone, Bone, true);

		//Calculate the impulse for the Knockback
		FVector TotalImpluseKnockback = UKismetMathLibrary::Multiply_VectorFloat(Impulse, ImpulseKnockback);
		//Apply impulse to the owner to make knockbac effect
		Owner->GetCharacterMovement()->AddImpulse(FVector(TotalImpluseKnockback.X, TotalImpluseKnockback.Y, 0.0f), true);
		
		//If the distance traveled in a instant its greater than the threshold tolerance to knockback the owner enters on ragdoll mode
		FVector FinalLocation = (InitialLocation - Owner->GetActorLocation().GetAbs()).GetAbs();
		
		AccumulatedKnockback += KnockbackImpulse;//Accumulates knockback

		FTimerHandle TM;
		Owner->GetWorld()->GetTimerManager().SetTimer(TM, [&] {
			//Checks if its not dead and if exceed the knockback tolerance if so then calculates if resist the knockback based on a probability with the excess of the accumulated knockback(included) to make it a little less predicible
			if (HealthComponentData.bIsDead == false && HealthComponentData.KnockbackToleranceThreshold != -1 && AccumulatedKnockback >= HealthComponentData.KnockbackToleranceThreshold && !CanResistKnockback(AccumulatedKnockback, HealthComponentData.KnockbackToleranceThreshold)) {
				//Disables the ragdoll of the hit reaction
				Owner->GetMesh()->SetAllBodiesBelowSimulatePhysics("Pelvis", false, true);
				HealthComponentData.bStartRecoveryFromHit = false;
				//Disable physics
				Owner->GetMesh()->SetSimulatePhysics(false);
				Owner->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

				//Restarts the ragdoll state
				Owner->ReplicatedRagdollStart();
			}
			}, 0.001f,false);
		
		//Starts timer to reset the accumulated knockback
		if (HealthComponentData.bIsStartingRecoverFromKnockback == false) {
			HealthComponentData.bIsStartingRecoverFromKnockback = true;
			Owner->GetWorld()->GetTimerManager().SetTimer(TimerHandleEndAccumulateKnockback, [&]() {
				AccumulatedKnockback = 0;
				HealthComponentData.bIsStartingRecoverFromKnockback = false;
				Owner->GetWorld()->GetTimerManager().ClearTimer(TimerHandleEndAccumulateKnockback);
			}, HealthComponentData.KnockbackTimeRecover, false);
		}

		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Health %f"), HealthComponentData.Life));

	}
}

bool UALSHealth_Component::CanResistKnockback(float KnockbackForce, float KnockbackTolerance) {
	if (KnockbackForce >= KnockbackTolerance) {
		//Calculates the excess force
		float ExcessKnockback = KnockbackForce - KnockbackTolerance;

		//Base probabilities to work with
		float TotalProbabilityNotKnockout = 20.0f;
		float TotalProbabilityKnockout = 80.0f;

		float ProbabilityKnockout = 0.0f;

		//If the excess its greater than the tolerance the the final probability to knock out its 100% (this is when a explosion occurs or something with high forces)
		if (ExcessKnockback > KnockbackTolerance) {
			ProbabilityKnockout = TotalProbabilityNotKnockout;
		}
		else {
			ProbabilityKnockout = (ExcessKnockback / KnockbackTolerance) * 100.0f;
		}

		//Substract the probability to the not knockout and adds it to the probability to knockout
		TotalProbabilityNotKnockout = TotalProbabilityNotKnockout - ProbabilityKnockout;
		TotalProbabilityKnockout = TotalProbabilityKnockout + ProbabilityKnockout;

		float RandomNumber = FMath::RandRange(1, 100);

		if (RandomNumber <= TotalProbabilityNotKnockout) {
			return true;
		}
		else {
			return false;
		}
	}

	return true;
}


void UALSHealth_Component::FallDamage_Implementation(float Damage, const FHitResult& Hit) {
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Some debug message!"));
	

	//Apply damage
	AddDamage(Damage);

	//Stops the audio and sets to nullptr for to be collected by the GC
	if (Owner->AudioFallingRefence) {
		Owner->AudioFallingRefence->Stop();
		Owner->AudioFallingRefence = nullptr;
	}

	//Spawns decal
	int RandomDecal = FMath::RandRange(0, BloodSplatDecalsMaterial.Num() - 1);
	if (BloodSplatDecalsMaterial.Num() > 0 && BloodSplatDecalsMaterial[RandomDecal].LoadSynchronous()) {
		FVector FinalDecalSize = DecalSize;

		//If the impacted zone is an arm or leg the blood splat its more small because there is less blood there
		if (Owner->GetMesh()->BoneIsChildOf(Hit.BoneName, "Thigh_R") || Owner->GetMesh()->BoneIsChildOf(Hit.BoneName, "Thigh_L") || Owner->GetMesh()->BoneIsChildOf(Hit.BoneName, "UpperArm_L") || Owner->GetMesh()->BoneIsChildOf(Hit.BoneName, "UpperArm_R")) {
			FinalDecalSize /= 3;
		}

		UDecalComponent* SpawnedDecal = UGameplayStatics::SpawnDecalAtLocation(Owner->GetWorld(), BloodSplatDecalsMaterial[RandomDecal].Get(), FinalDecalSize, Hit.ImpactPoint + DecalLocationOffset, UKismetMathLibrary::MakeRotFromX(Hit.Normal) + DecalRotationOffset, DecalLifeSpan);
		SpawnedDecal->FadeScreenSize = -8.0f;
		SpawnedDecal->SetFadeOut(DecalLifeSpan - DecalFadeOut, DecalFadeOut, false);
	}

	//Spawn sound pain character
	if (PainSoundCue && HealthComponentData.bIsDead == false) {
		UGameplayStatics::SpawnSoundAttached(PainSoundCue, Owner->GetMesh());
	}

	//Spawns blood splat particle
	if (BloodSplatParticle.LoadSynchronous()) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(Owner->GetWorld(), BloodSplatParticle.Get(), Hit.ImpactPoint + BloodSplatParticleLocationOffset, UKismetMathLibrary::MakeRotFromX(Hit.Normal) + BloodSplatParticleRotationOffset);
	}

	//Play camera shake, check if the controller is not null to avoid crashes
	if (bActivateCamerashakeController == true && HealthComponentData.bIsDead == false && FallImpactCameraShake && Cast<APlayerController>(Owner->GetController())) {
		Cast<APlayerController>(Owner->GetController())->ClientStartCameraShake(FallImpactCameraShake, ScaleImpactCamShake);
	}

	//Execute the feedback force
	if (bActivateFeedbackController == true && HealthComponentData.bIsDead == false && ImpactFallingFeedbackForce) {
		UGameplayStatics::GetPlayerController(Owner->GetWorld(), 0)->ClientPlayForceFeedback(ImpactFallingFeedbackForce);
	}

	//Impact surface FX
	FALSFallImpactFX* ImpactFX = nullptr;
	//Gets all the rows of the data table given
	if (FallingImpactDataTable) {
		FallingImpactDataTable->GetAllRows<FALSFallImpactFX>(FString(), FallImpactFXArray);
	}

	if (FallImpactFXArray.Num() > 0 && Hit.PhysMaterial.Get()) {
		//Ballistic FX
		EPhysicalSurface SurfaceType = Hit.PhysMaterial.Get()->SurfaceType;

		if (auto FoundResult = FallImpactFXArray.FindByPredicate([&](const FALSFallImpactFX* Value) {return SurfaceType == Value->SurfaceType; })) {
			ImpactFX = *FoundResult;
		}
		else {
			return;
		}

		//Play sound
		if (ImpactFX->Sound.LoadSynchronous()) {
			UGameplayStatics::SpawnSoundAtLocation(Owner->GetWorld(), ImpactFX->Sound.Get(), Hit.ImpactPoint + ImpactFX->SoundLocationOffset, ImpactFX->SoundRotationOffset);
		}

		//Play particle system
		if (ImpactFX->NiagaraSystem.LoadSynchronous()) {
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(Owner->GetWorld(), ImpactFX->NiagaraSystem.Get(), Hit.ImpactPoint + ImpactFX->NiagaraLocationOffset, UKismetMathLibrary::MakeRotFromX(Hit.Normal) + ImpactFX->NiagaraRotationOffset);
		}
	}
}

void UALSHealth_Component::GetHealthComponentData_Implementation(FHealthComponentData& HealthComponentStruct) {
	HealthComponentStruct = HealthComponentData;
}

void UALSHealth_Component::SetHealthComponentData_Implementation(FHealthComponentData HealthComponentStruct) {
	HealthComponentData = HealthComponentStruct;
}

void UALSHealth_Component::AddDamage(float Damage) {
	if (HealthComponentData.bIsInvulnerable == false) {
		HealthComponentData.Life -= Damage;
	}

	//Activates the damage post process effect
	//if (HealthComponentData.Life <= HealthPostProcessThreshold && bActivatePostProcess == true) {
	//	PostProcessHurted->BlendWeight = 1.0f;
	//}
	//Enables postProcess effect
	if (bActivatePostProcess == true) {
		PostProcessHurted->BlendWeight = 1.0f;
		Owner->GetWorld()->GetTimerManager().ClearTimer(TimerHandleEnablePostProccessFade);
		bFadeOutPostProcess = false;
		if (HealthComponentData.Life > HealthPostProcessThreshold) {
			Owner->GetWorld()->GetTimerManager().SetTimer(TimerHandleEnablePostProccessFade, [&]() {
				bFadeOutPostProcess = true;
				Owner->GetWorld()->GetTimerManager().ClearTimer(TimerHandleEnablePostProccessFade);
				}, DelayFadeOutPostProcessEffect, false);
		}
	}

	if (HealthComponentData.Life <= 0 && HealthComponentData.bIsDead == false) {
		HealthComponentData.Life = 0;
		Dead();
	}
}

void UALSHealth_Component::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	//If hit something different of himself(the character mesh0) and his speed its greater than a threshold
	if (Owner->GetMovementState() == EALSMovementState::InAir || Owner->GetMovementState() == EALSMovementState::Ragdoll) {
		float DistanceTraveled = FMath::RoundToFloat(FMath::Abs((Owner->PositionFalling - Owner->GetActorLocation()).Size()));

		if (bIsHittedFall == false && OtherComp != Owner->GetMesh() && DistanceTraveled > HealthComponentData.FallDamageThreshold) {
			bIsHittedFall = true;
			
			//float SpeedRagdoll = DistanceTraveled; //FVector(Owner->GetLastRagdollVelocity().X, Owner->GetLastRagdollVelocity().Y, Owner->GetLastRagdollVelocity().Z).Size();
			FallDamage_Implementation((DistanceTraveled * HealthComponentData.FallDamageMultiplicator) / HealthComponentData.FallDamageThreshold, Hit);

			Owner->GetWorld()->GetTimerManager().SetTimer(TimerHandleFallRagdollRestart, [&]() {
				bIsHittedFall = false;
				Owner->GetWorld()->GetTimerManager().ClearTimer(TimerHandleFallRagdollRestart);
				}, HealthComponentData.TimeFallDamageRestart, false);

			//Its not outside the condition because for give it a more realistic sentation it must accumulate the potential damage until accomplish the condition, with this the player can pontentially be harmmed while its sliding thorught a ramp, if its update all the time it will just slide
			Owner->PositionFalling = Owner->GetActorLocation();
		}

		//If the speed of the ragdoll is less than the threshold then updates the postionFalling to avoid potential accumulative damage when its sliding from a ramp slowly
		if (FVector(Owner->GetLastRagdollVelocity().X, Owner->GetLastRagdollVelocity().Y, Owner->GetLastRagdollVelocity().Z).Size() < HealthComponentData.FallDamageThreshold) {
			Owner->PositionFalling = Owner->GetActorLocation();
		}
	}
}

void UALSHealth_Component::Dead() {
	HealthComponentData.bIsDead = true;

	if (HealthComponentData.bDestroyOnDeath == true) {
		Owner->GetWorld()->GetTimerManager().SetTimer(TimerHandleDestroyActor, [&]() {
			Owner->Destroy();
			Owner->GetWorld()->GetTimerManager().ClearTimer(TimerHandleDestroyActor);
			}, HealthComponentData.TimeDestroyOnDeath, false);
	}

	//Spawn sound death effect
	if (DeathEffectSoundCue) {
		UGameplayStatics::SpawnSoundAttached(DeathEffectSoundCue, Owner->GetMesh());
	}

	//Fade sound
	if (bFadeAudioDeath == true) {
		UGameplayStatics::SetSoundMixClassOverride(Owner->GetWorld(), FadeSoundMixer, FadeSoundClass, FadeSoundVolume, 1.0f, FadeTime);
		UGameplayStatics::PushSoundMixModifier(Owner->GetWorld(), FadeSoundMixer);
	}

	//Creates the widget
	if (bCreateWidget == true) {
		CreateWidget();
	}

	//Sets the slow motion effect
	if (UKismetSystemLibrary::IsStandalone(this) && bActivateSlowMotionOnDead == true) {
		UGameplayStatics::SetGlobalTimeDilation(this, DeathSlowMotion);
	}

	//Set the postporcess effect
	PostProcessDead->BlendWeight = 1.0f;

	//Set ragdoll mode
	if (Owner->GetMovementState() != EALSMovementState::Ragdoll) {
		//Disable the ragdoll from the hitreaction
		Owner->GetMesh()->SetAllBodiesBelowSimulatePhysics("Pelvis", false, true);
		HealthComponentData.bStartRecoveryFromHit = false;
		//Disable physics
		Owner->GetMesh()->SetSimulatePhysics(false);
		Owner->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		//Restarts the ragdoll again
		Owner->ReplicatedRagdollStart();
	}

	//Disable input of the character
	if (HealthComponentData.bDisableInputCharacter == true) {
		Owner->GetWorld()->GetFirstPlayerController()->DisableInput(Owner->GetWorld()->GetFirstPlayerController());
	}
}

void UALSHealth_Component::CreateWidget_Implementation() {
}

