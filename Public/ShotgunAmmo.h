// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GeneralStructLibrary.h"
#include "Interfaces/IProjectile_Basic.h"
#include "Interfaces/IHealthComponent.h"

#include "DrawDebugHelpers.h"
#include "Library/ALSCharacterStructLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/DecalComponent.h"
#include "CollisionQueryParams.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Engine/StaticMesh.h"
#include "Particles/ParticleSystem.h" 
#include "ShotgunAmmo.generated.h"

class ALSCharacter;

UCLASS()
class ALSV4_CPP_API AShotgunAmmo : public AActor, public IIProjectile_Basic
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShotgunAmmo();

	//IIProjectile_Basic
	virtual void Fire_Implementation(FVector InitLocation) override;
	virtual void GetProjectileData_Implementation(FProjectileData& ProjectileStruct) override;
	virtual void SetProjectileData_Implementation(FProjectileData ProjectileStruct) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Properties | FX")
		UNiagaraSystem* BulletTraceEffect;

	UPROPERTY(EditAnywhere, Category = "Properties | FX")
		FName BulletTraceEffectParameterName;

	UPROPERTY(EditAnywhere, Category = "Properties | FX")
		FVector BulletTraceEffectLocationOffset;

	UPROPERTY(EditAnywhere, Category = "Properties | FX")
		FRotator BulletTraceEffectRotationOffset;

public:
	UPROPERTY(EditAnywhere, Category = "Properties")
		FProjectileData ProjectileData;

	UPROPERTY(EditAnywhere, Category = "Properties")
		TEnumAsByte<ETraceTypeQuery> TraceChannel;

	UPROPERTY(EditAnywhere, Category = "Properties")
		float WidthBulletCollision = 1.f;

private:
	TArray<FALSBallisticHitFX*> BallisticFXArray;
};
