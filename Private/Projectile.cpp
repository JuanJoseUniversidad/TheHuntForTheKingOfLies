// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/StaticMesh.h"
#include "NiagaraComponent.h"
#include "Components/DecalComponent.h"
#include "Engine/DataTable.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Library/ALSCharacterStructLibrary.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->IgnoreActorWhenMoving(GetOwner(), true);
	//Disable the collision
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	CollisionComp->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	CollisionComp->SetupAttachment(RootComponent);
	

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComp"));
	ProjectileMesh->SetupAttachment(CollisionComp);

	ParticleComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ParticleComp"));
	ParticleComponent->SetupAttachment(CollisionComp);

}

void AProjectile::SetShooterOwner(AActor* NewOwner) {
	ShooterOwner = NewOwner;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	//Gets all the rows of the data table given
	ImpactFXDataTable->GetAllRows<FALSBallisticHitFX>(FString(), ImpactFXArray);

	FTimerHandle EnableCollisionTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(EnableCollisionTimerHandle, [&] {
		//Set the owner of the weapon as an actor to ignore by the proyectile
		CollisionComp->IgnoreActorWhenMoving(ShooterOwner, true);
		//Enable the collision
		CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}, 0.1f, false);
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	//Try to get the health component and its interface (if there is one component), call the function to make a hit on the actor with this component
	if (!Hit.GetActor()->GetComponentsByInterface(UIHealthComponent::StaticClass()).IsEmpty()) {
		UActorComponent* HealthComponent = Hit.GetActor()->GetComponentsByInterface(UIHealthComponent::StaticClass())[0];
		IIHealthComponent* HealthComponent_Interface = Cast<IIHealthComponent>(HealthComponent);
		HealthComponent_Interface->Execute_HitDamage(HealthComponent, Damage, Hit, BoneImpulse, KnockbackImpulse);
	}

	//Ballistic fx
	FALSBallisticHitFX* HitFX = nullptr;

	if (Hit.PhysMaterial.Get()) {
		
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
			//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitFX->NiagaraSystem.Get(), Hit.ImpactPoint + HitFX->NiagaraLocationOffset, UKismetMathLibrary::MakeRotFromX(Hit.Normal) + HitFX->NiagaraRotationOffset);
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

	Destroy();
}



