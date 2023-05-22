// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_Range_Projectiles.h"
#include "Animation/AnimMontage.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "NiagaraSystem.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AWeapon_Range_Projectiles::AWeapon_Range_Projectiles()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = SceneComponent;

	WeaponSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkeletalMesh"));
	WeaponSkeletalMesh->SetupAttachment(SceneComponent);

	Dispersion = 30.0f;
	MuzzleSocketName = "Muzzle";
}

float AWeapon_Range_Projectiles::TryToUse_Implementation(int32 UseMode) {
	if (UseMode > EquipableObjectData_Basic.UseMontages.Num() - 1) {
		UseMode = EquipableObjectData_Basic.UseMontages.Num() - 1;
	}
	else if (UseMode < 0) {
		UseMode = 0;
	}

	if (EquipableObjectData_Basic.UseMontages.Num() > 0) {
		//Play use animation
		Cast<ACharacter>(GetOwner())->GetMesh()->GetAnimInstance()->Montage_Play(EquipableObjectData_Basic.UseMontages[UseMode], 1.0f);
		return EquipableObjectData_Basic.UseMontages[UseMode]->GetPlayLength();
	}
	else {
		return -1.0f;
	}
}

bool AWeapon_Range_Projectiles::Use_Implementation() {

	FVector EyesLocation;
	FRotator EyesRotation;
	GetOwner()->GetActorEyesViewPoint(EyesLocation, EyesRotation);

	FHitResult HitResult;

	//Gets the impact point to where the projectile must be ahead
	FVector ImpactPoint;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, EyesLocation, EyesLocation+(EyesRotation.Vector() * 24000.0f), ECollisionChannel::ECC_Visibility)) {
		ImpactPoint = HitResult.ImpactPoint;
	}
	else {
		ImpactPoint = HitResult.TraceEnd;
	}

	//Calculate a dispersion factor
	ImpactPoint = ImpactPoint + FVector(FMath::RandRange(-Dispersion, Dispersion), 0.0f, FMath::RandRange(0.0f, Dispersion));
	
	DrawDebugLine(GetWorld(), EyesLocation, ImpactPoint, FColor::Red, true);

	//Calculate the location and rotation to define the projectile's trayectory
	FTransform TransformSpawn;
	TransformSpawn.SetLocation(WeaponSkeletalMesh->GetSocketLocation(MuzzleSocketName));
	TransformSpawn.SetRotation(UKismetMathLibrary::FindLookAtRotation(WeaponSkeletalMesh->GetSocketLocation(MuzzleSocketName), ImpactPoint).Quaternion());

	//Spawns the projectile
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, TransformSpawn, SpawnInfo);
	Projectile->SetOwner(this);
	Projectile->SetShooterOwner(GetOwner());

	//Muzzle effect
	if (MuzzleEffect) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), MuzzleEffect, WeaponSkeletalMesh->GetSocketLocation(MuzzleSocketName), WeaponSkeletalMesh->GetSocketRotation(MuzzleSocketName));
	}

	//Shoot sound
	if (UseSoundCue) {
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), UseSoundCue, GetActorLocation());
	}

	//Plays weapon animation
	if (WeaponAnimation) {
		WeaponSkeletalMesh->GetAnimInstance()->Montage_Play(WeaponAnimation);
	}

	return true;
}

void AWeapon_Range_Projectiles::SetVisibleMesh_Implementation(bool Visible) {
	WeaponSkeletalMesh->SetVisibility(Visible);
}

void AWeapon_Range_Projectiles::PlayReadySound_Implementation() {
	if (bCanPlayAimSound == true) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), this->AimSoundCue, GetActorLocation());
		bCanPlayAimSound = false;
	}
	GetWorld()->GetTimerManager().ClearTimer(TimerHandleAimSound);
}

void AWeapon_Range_Projectiles::StopReadySound_Implementation() {
	GetWorld()->GetTimerManager().SetTimer(TimerHandleAimSound, [&]() {
		bCanPlayAimSound = true;
	}, 0.5f, false);
}

void AWeapon_Range_Projectiles::GetEquipableObjectData_Basic_Implementation(FEquipableObjectData_Basic& EquipableObjectStruct) {
	EquipableObjectStruct = EquipableObjectData_Basic;
}

void AWeapon_Range_Projectiles::SetEquipableObjectData_Basic_Implementation(FEquipableObjectData_Basic EquipableObjectStruct) {
	EquipableObjectData_Basic = EquipableObjectStruct;
}

// Called when the game starts or when spawned
void AWeapon_Range_Projectiles::BeginPlay()
{
	Super::BeginPlay();
	
}

