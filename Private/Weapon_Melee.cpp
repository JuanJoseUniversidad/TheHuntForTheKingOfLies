// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_Melee.h"
#include "GeneralStructLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Engine/World.h"
#include "Components/SceneComponent.h"
#include "CollisionShape.h"
#include "Interfaces/IHealthComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/DecalComponent.h"
#include "CollisionQueryParams.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Library/ALSCharacterStructLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AWeapon_Melee::AWeapon_Melee()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMeshComponent"));
	WeaponMesh->SetupAttachment(SceneComponent);

	InitBoxSocketName = "InitTraceSocket";
	EndBoxSocketName = "EndTraceSocket";

	SectionMontageUsageName = "Using";

	TraceRadius = 5.0f;
	TimeForNextTrace = 0.0001f;

	bCanPlayAimSound = true;
	bIsUsing = false;

}

float AWeapon_Melee::TryToUse_Implementation(int32 UseMode) {
	if (UseMode > EquipableObjectData_Basic.UseMontages.Num() - 1) {
		UseMode = EquipableObjectData_Basic.UseMontages.Num() - 1;
	}
	else if (UseMode < 0) {
		UseMode = 0;
	}

	if (EquipableObjectData_Basic.UseMontages.Num() > 0) {
		Cast<ACharacter>(GetOwner())->GetMesh()->GetAnimInstance()->Montage_Play(EquipableObjectData_Basic.UseMontages[UseMode], 1.0f);
		SectionUsageDuration = EquipableObjectData_Basic.UseMontages[UseMode]->GetSectionLength(EquipableObjectData_Basic.UseMontages[UseMode]->GetSectionIndex(SectionMontageUsageName));
		return EquipableObjectData_Basic.UseMontages[UseMode]->GetPlayLength();
	}
	else {
		return -1.0f;
	}
}

bool AWeapon_Melee::Use_Implementation() {
	//A flip flop which alternates in use mode and not use mode (this is due that make a notify state its not reliable and consistent)
	//if (bIsUsing == false) {
	//	bIsUsing = true;
		GetWorld()->GetTimerManager().SetTimer(TimerHandleUse, [&] {
			TArray<FHitResult> Hits;

			bool bHit = UKismetSystemLibrary::SphereTraceMulti(GetWorld(), WeaponMesh->GetSocketLocation(InitBoxSocketName), WeaponMesh->GetSocketLocation(EndBoxSocketName), TraceRadius, TraceChannel, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, Hits, true);
			//DrawDebugSphere(GetWorld(), (WeaponMesh->GetSocketLocation(InitBoxSocketName) + WeaponMesh->GetSocketLocation(EndBoxSocketName)) / 2, 5.0f, 6, FColor::Red, true, 15.0);
			//DrawDebugBox(GetWorld(), WeaponMesh->GetSocketLocation(InitBoxSocketName), WeaponMesh->GetSocketLocation(EndBoxSocketName), FQuat(GetActorRotation()), FColor::Cyan, true, 15.0f, (uint8)0U, 4.0f);
			if (bHit) {
				for (auto& Hit : Hits) {
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, Hit.GetActor()->GetName());
					//Try to get the health component and its interface (if there is one component), call the function to make a hit on the actor with this component
					if (!Hit.GetActor()->GetComponentsByInterface(UIHealthComponent::StaticClass()).IsEmpty()) {
						UActorComponent* HealthComponent = Hit.GetActor()->GetComponentsByInterface(UIHealthComponent::StaticClass())[0];
						IIHealthComponent* HealthComponent_Interface = Cast<IIHealthComponent>(HealthComponent);
						HealthComponent_Interface->Execute_HitDamage(HealthComponent, Damage, Hit, BoneImpulse, KnockbackImpulse);
					}

					if (Hit.PhysMaterial.Get()) {
						//Ballistic fx
						FALSBallisticHitFX* HitFX = nullptr;

						//Ballistic FX
						EPhysicalSurface SurfaceType = Hit.PhysMaterial.Get()->SurfaceType;

						if (auto FoundResult = ImpactFXArray.FindByPredicate([&](const FALSBallisticHitFX* Value) {return SurfaceType == Value->SurfaceType; })) {
							HitFX = *FoundResult;
						}
						else {
							return;
						}

						//Play sound
						if (HitFX->Sound.LoadSynchronous()) {
							UGameplayStatics::SpawnSoundAtLocation(GetWorld(), HitFX->Sound.Get(), Hit.ImpactPoint + HitFX->SoundLocationOffset, HitFX->SoundRotationOffset);
						}

						//Play particle system
							//Niagara particle system
						if (HitFX->NiagaraSystem.LoadSynchronous()) {
							UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitFX->NiagaraSystem.Get(), Hit.ImpactPoint + HitFX->NiagaraLocationOffset, UKismetMathLibrary::MakeRotFromX(Hit.Normal) + HitFX->NiagaraRotationOffset);
						}
							//Cascade particle system
						if (HitFX->CascadeSystem.LoadSynchronous()) {	
							UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitFX->CascadeSystem.Get(), Hit.ImpactPoint + HitFX->CascadeLocationOffset, UKismetMathLibrary::MakeRotFromX(Hit.Normal) + HitFX->CascadeRotationOffset, true);

						}

						//Spawn decal
						int randomDecal = FMath::RandRange(0, HitFX->DecalsMaterial.Num() - 1);
						if (!HitFX->DecalsMaterial.IsEmpty() && HitFX->DecalsMaterial[randomDecal].LoadSynchronous()) {
							UDecalComponent* SpawnedDecal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), HitFX->DecalsMaterial[randomDecal].Get(), HitFX->DecalSize, Hit.ImpactPoint + HitFX->DecalLocationOffset, UKismetMathLibrary::MakeRotFromX(Hit.Normal) + HitFX->DecalRotationOffset, HitFX->DecalLifeSpan);
							SpawnedDecal->FadeScreenSize = -8.0f;
							SpawnedDecal->SetFadeOut(HitFX->DecalLifeSpan - HitFX->DecalFadeOut, HitFX->DecalFadeOut, false);
						}
					}
				}
			}
		}, TimeForNextTrace, true);

		GetWorld()->GetTimerManager().SetTimer(TimerHandleResetUse, [&] {
			GetWorld()->GetTimerManager().ClearTimer(TimerHandleUse);
		}, SectionUsageDuration, false);
	//}
	//else {
	//	bIsUsing = false;
	//	GetWorld()->GetTimerManager().ClearTimer(TimerHandleUse);
	//}

	return true;
}

void AWeapon_Melee::SetVisibleMesh_Implementation(bool Visible) {
	WeaponMesh->SetVisibility(Visible);
}

void AWeapon_Melee::PlayReadySound_Implementation() {
	if (bCanPlayAimSound == true) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), this->AimSoundCue, GetActorLocation());
		bCanPlayAimSound = false;
	}
	GetWorld()->GetTimerManager().ClearTimer(TimerHandleAimSound);
}

void AWeapon_Melee::StopReadySound_Implementation() {
	GetWorld()->GetTimerManager().SetTimer(TimerHandleAimSound, [&]() {
		bCanPlayAimSound = true;
	}, 0.5f, false);
}

void AWeapon_Melee::GetEquipableObjectData_Basic_Implementation(FEquipableObjectData_Basic& EquipableObjectStruct) {
	EquipableObjectStruct = EquipableObjectData_Basic;
}

void AWeapon_Melee::SetEquipableObjectData_Basic_Implementation(FEquipableObjectData_Basic EquipableObjectStruct) {
	EquipableObjectData_Basic = EquipableObjectStruct;
}

// Called when the game starts or when spawned
void AWeapon_Melee::BeginPlay()
{
	Super::BeginPlay();
	
	//Gets all the rows of the data table given
	ImpactFXDataTable->GetAllRows<FALSBallisticHitFX>(FString(), ImpactFXArray);


	TraceQueryParams.AddIgnoredActor(this);
	TraceQueryParams.AddIgnoredActor(GetOwner());
	TraceQueryParams.bReturnPhysicalMaterial = true;
	TraceQueryParams.bTraceComplex = false;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(GetOwner());

}

void AWeapon_Melee::Tick(float DeltaTime) {

}

