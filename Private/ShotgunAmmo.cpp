// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotgunAmmo.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"

// Sets default values
AShotgunAmmo::AShotgunAmmo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BulletTraceEffectParameterName = "User.ImpactPositions";
}

// Called when the game starts or when spawned
void AShotgunAmmo::BeginPlay()
{
	Super::BeginPlay();
	
	//Gets all the rows of the data table given
	ProjectileData.BallisticFXDataTable->GetAllRows<FALSBallisticHitFX>(FString(), BallisticFXArray);
}

// Called every frame
void AShotgunAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#pragma region ProjectileBasic
void AShotgunAmmo::Fire_Implementation(FVector InitLocation) {
	//Ballistic fx
	FALSBallisticHitFX* HitFX = nullptr;
	//Array of actors to ignore
	TArray<AActor*> ActorsToIgnore;

	//Hits list for bullet trace effect
	TArray<FVector> HitLocations;

	//Gun line trace
	for (int32 i = 0; i < ProjectileData.NumberOfShoots; i++) {

		//Camera reference line trace
		FVector CamLocation = UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->GetCameraLocation();
		FVector CamForVector = UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->GetCameraRotation().Vector();
		FHitResult CamOutHit;
			//FCollisionQueryParams CamCollisionParams;
		//Calculate the spread
		FVector SpreadVector;
		SpreadVector.X = FMath::RandRange(ProjectileData.Spread * -1, ProjectileData.Spread);
		SpreadVector.Y = FMath::RandRange(ProjectileData.Spread * -1, ProjectileData.Spread);
		SpreadVector.Z = FMath::RandRange(ProjectileData.Spread * -1, ProjectileData.Spread);

			//bool bCamHit = GetWorld()->LineTraceSingleByChannel(CamOutHit, CamLocation, ((CamForVector * ProjectileData.Range) + CamLocation) + SpreadVector, TraceChannel, CamCollisionParams);
		//Trace a capsule form like which will help the player for the precision
		bool bCamHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), CamLocation, ((CamForVector * ProjectileData.Range) + CamLocation) + SpreadVector, WidthBulletCollision, TraceChannel, true, ActorsToIgnore, EDrawDebugTrace::None, CamOutHit, true);

		FVector EndTrace;
		if (bCamHit) {
			EndTrace = CamOutHit.ImpactPoint + (CamForVector+ FVector(0.1f, 0.05f, 0.0f));
		}
		else {
			EndTrace = CamOutHit.TraceEnd + (CamForVector + FVector(0.1f, 0.05f, 0.0f));
		}
			//DrawDebugBox(GetWorld(), CamOutHit.ImpactPoint, FVector(10, 10, 10), FColor::Red, true);
			//DrawDebugLine(GetWorld(), CamLocation, ((CamForVector * ProjectileData.Range) + CamLocation) + SpreadVector, FColor::Red, true);
			//endTrace += spreadVector;

		FHitResult ShootOutHit;
		FCollisionQueryParams ShootCollisionParams;
		ShootCollisionParams.bReturnPhysicalMaterial = true;
		ShootCollisionParams.bTraceComplex = false;

			//bool bShootHit = GetWorld()->LineTraceSingleByChannel(ShootOutHit, InitLocation, EndTrace, TraceChannel, ShootCollisionParams);
		//Trace a capsule form like which will help the player for the precision
		bool bShootHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), InitLocation, EndTrace, WidthBulletCollision, TraceChannel, false, ActorsToIgnore, EDrawDebugTrace::None, ShootOutHit, true, FLinearColor::Blue);
		if (bShootHit == false) {
				//ShootCollisionParams.bTraceComplex = true;
				//bShootHit = GetWorld()->LineTraceSingleByChannel(ShootOutHit, InitLocation, EndTrace, TraceChannel, ShootCollisionParams);
			bShootHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), InitLocation, EndTrace, WidthBulletCollision, TraceChannel, true, ActorsToIgnore, EDrawDebugTrace::None, ShootOutHit, true, FLinearColor::Blue);
		}

			//DrawDebugSphere(GetWorld(), ShootOutHit.ImpactPoint, 5, 5, FColor::Green,true,10,2,2);
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("%d,%d,%d"), ShootOutHit.ImpactPoint.X, ShootOutHit.ImpactPoint.Y, ShootOutHit.ImpactPoint.Z));
		
		

		if (bShootHit) {

			//Adds the hit impact
			HitLocations.Add(ShootOutHit.ImpactPoint);

				//DrawDebugBox(GetWorld(), ShootOutHit.ImpactPoint, FVector(10, 10, 10), FColor::Blue, true);
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, ShootOutHit.GetActor()->GetName());
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Magenta, FString::Printf(TEXT("%d,%d,%d"), ShootOutHit.ImpactPoint.X, ShootOutHit.ImpactPoint.Y, ShootOutHit.ImpactPoint.Z));
			//Try to get the health component and its interface (if there is one component), call the function to make a hit on the actor with this component
			if (!ShootOutHit.GetActor()->GetComponentsByInterface(UIHealthComponent::StaticClass()).IsEmpty()) {
				UActorComponent* HealthComponent = ShootOutHit.GetActor()->GetComponentsByInterface(UIHealthComponent::StaticClass())[0];
				IIHealthComponent* HealthComponent_Interface = Cast<IIHealthComponent>(HealthComponent);
				HealthComponent_Interface->Execute_HitDamage(HealthComponent, ProjectileData.Damage, ShootOutHit, ProjectileData.BoneImpulse, ProjectileData.KnockbackImpulsePerShoot);
			}

			if (ShootOutHit.PhysMaterial.Get()) {
				//Ballistic FX
				EPhysicalSurface SurfaceType = ShootOutHit.PhysMaterial.Get()->SurfaceType;

				if (auto FoundResult = BallisticFXArray.FindByPredicate([&](const FALSBallisticHitFX* Value) {return SurfaceType == Value->SurfaceType; })) {
					HitFX = *FoundResult;
				}
				else {
					return;
				}

				//Play sound
				if (HitFX->Sound.LoadSynchronous()) {
					UGameplayStatics::SpawnSoundAtLocation(GetWorld(), HitFX->Sound.Get(), ShootOutHit.ImpactPoint + HitFX->SoundLocationOffset, HitFX->SoundRotationOffset);
				}

				//Play particle system
					//Niagara particle system
				if (HitFX->NiagaraSystem.LoadSynchronous()) {
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitFX->NiagaraSystem.Get(), ShootOutHit.ImpactPoint + HitFX->NiagaraLocationOffset, UKismetMathLibrary::MakeRotFromX(ShootOutHit.Normal) + HitFX->NiagaraRotationOffset);
				}
					//Cascade particle system
				if (HitFX->CascadeSystem.LoadSynchronous()) {
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitFX->CascadeSystem.Get(), ShootOutHit.ImpactPoint + HitFX->CascadeLocationOffset, UKismetMathLibrary::MakeRotFromX(ShootOutHit.Normal) + HitFX->CascadeRotationOffset, true);
				}


				//Spawn decal
				int randomDecal = FMath::RandRange(0, HitFX->DecalsMaterial.Num() - 1);
				if (!HitFX->DecalsMaterial.IsEmpty() && HitFX->DecalsMaterial[randomDecal].LoadSynchronous()) {
					UDecalComponent* SpawnedDecal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), HitFX->DecalsMaterial[randomDecal].Get(), HitFX->DecalSize, ShootOutHit.ImpactPoint + HitFX->DecalLocationOffset, UKismetMathLibrary::MakeRotFromX(ShootOutHit.Normal) + HitFX->DecalRotationOffset, HitFX->DecalLifeSpan);
					SpawnedDecal->FadeScreenSize = -8.0f;
					SpawnedDecal->SetFadeOut(HitFX->DecalLifeSpan - HitFX->DecalFadeOut, HitFX->DecalFadeOut, false);
				}
			}
		}
		else {
			//Adds the en trace in case that nothing has hitted
			HitLocations.Add(EndTrace);
		}
			//DrawDebugLine(GetWorld(), InitLocation, EndTrace, FColor::Blue, true);
	}

	//Bullet linetrace effect
	if (ensure(BulletTraceEffect)) {
		UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BulletTraceEffect, InitLocation + BulletTraceEffectLocationOffset, BulletTraceEffectRotationOffset);
		UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(NiagaraComp, BulletTraceEffectParameterName, HitLocations);
	}
}

void AShotgunAmmo::GetProjectileData_Implementation(FProjectileData& ProjectileStruct) {
	ProjectileStruct = ProjectileData;
}

void AShotgunAmmo::SetProjectileData_Implementation(FProjectileData ProjectileStruct) {
	ProjectileData = ProjectileStruct;
}
#pragma endregion
