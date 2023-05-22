// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IHealthComponent.h"
#include "GeneralStructLibrary.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UNiagaraComponent;
class UDataTable;
struct FALSBallisticHitFX;

UCLASS(Blueprintable, BlueprintType)
class ALSV4_CPP_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	void SetShooterOwner(AActor* NewOwner);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


	UPROPERTY(VisibleAnywhere, Category = "Components")
		UProjectileMovementComponent* ProjectileMovementComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UNiagaraComponent* ParticleComponent;


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

private:
	UPROPERTY()
		AActor* ShooterOwner; //The owner of the weapon who shooted the projectile
	
	TArray<FALSBallisticHitFX*> ImpactFXArray;
};
