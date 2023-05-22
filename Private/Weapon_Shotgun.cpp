// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_Shotgun.h"

// Sets default values
AWeapon_Shotgun::AWeapon_Shotgun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SceneComp->SetupAttachment(RootComponent);

	ShotgunMesh = CreateDefaultSubobject< USkeletalMeshComponent>(TEXT("Shotgun_SM"));
	ShotgunMesh->SetupAttachment(SceneComp);

	//Sets magazine size
	Magazine.SetNum(MagazineSize);
}

// Called when the game starts or when spawned
void AWeapon_Shotgun::BeginPlay()
{
	Super::BeginPlay();
	
	//Load the next bullet on the chamber
	SetReadyLoad_Implementation();
}

// Called every frame
void AWeapon_Shotgun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#pragma region EquipableObject_Basic
float AWeapon_Shotgun::TryToUse_Implementation(int32 UseMode) {
	if (UseMode > EquipableObjectData_Basic.UseMontages.Num() - 1) {
		UseMode = EquipableObjectData_Basic.UseMontages.Num() - 1;
	}
	else if (UseMode < 0) {
		UseMode = 0;
	}

	if (EquipableObjectData_Reload.LoadReady/*m_gunChamber*/) {
		Cast<ACharacter>(GetOwner())->GetMesh()->GetAnimInstance()->Montage_Play(EquipableObjectData_Basic.UseMontages[UseMode], 1.0f);
		return EquipableObjectData_Basic.UseMontages[UseMode]->GetPlayLength();
	}
	else {
		//Dry shoot sound
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), DryUseSoundCue, GetActorLocation());
		return -1.0f;
	}
}

bool AWeapon_Shotgun::Use_Implementation() {
	//if (EquipableObjectData_Reload.LoadReady/*m_gunChamber*/) {
		FVector InitShoot = ShotgunMesh->GetSocketLocation(SocketMuzzleName);
		ProyectileBasic_Interface->Execute_Fire(/*m_gunChamber*/EquipableObjectData_Reload.LoadReady, InitShoot);

		//Recoild
		UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->AddControllerPitchInput(FMath::RandRange(-0.2f, Recoil*-1));
		UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->AddControllerYawInput(FMath::RandRange((Recoil / 3) * -1, Recoil / 3));

		//Camera shake
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(CameraShake, 1.0f);

		//Shoot sound
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), UseSoundCue, GetActorLocation());

		//Muzzle effect
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),MuzzleEffect, ShotgunMesh->GetSocketLocation(SocketMuzzleName), ShotgunMesh->GetSocketRotation(SocketMuzzleName));

		//Shell eject effect
		UNiagaraComponent* SpawnedParticle = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ShellEjectEffect, ShotgunMesh->GetSocketLocation(SocketShellEjectName), ShotgunMesh->GetSocketRotation(SocketShellEjectName));
		SpawnedParticle->SetVariableStaticMesh(NiagaraParamenterShellName, /*m_gunChamber->*/ProjectileData.Mesh);

		//Execute animation of the gun itself
		//if (GunMontage) {
			//ShotgunMesh->GetAnimInstance()->Montage_Play(GunMontage, 1.0f);
			ShotgunMesh->PlayAnimation(GunMontage, false);
		//}
		
		//Execute the feedback force
		if (ShootFeedbackForce) {
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->ClientPlayForceFeedback(ShootFeedbackForce);
		}

		/*m_gunChamber*/EquipableObjectData_Reload.LoadReady->Destroy();
		/*m_gunChamber*/EquipableObjectData_Reload.LoadReady = nullptr;
		ProyectileBasic_Interface = nullptr;


		return true;
	/*}
	else {
		//Dry shoot sound
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), DryUseSoundCue, GetActorLocation());

		return false;
	}*/

}

void AWeapon_Shotgun::SetVisibleMesh_Implementation(bool Visible) {
	ShotgunMesh->SetVisibility(Visible);
}

void AWeapon_Shotgun::PlayReadySound_Implementation() {
	if (bCanPlayAimSound == true) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), this->AimSoundCue, GetActorLocation());
		bCanPlayAimSound = false;
	}
	GetWorld()->GetTimerManager().ClearTimer(TimerHandleAimSound);
}

void AWeapon_Shotgun::StopReadySound_Implementation() {
	GetWorld()->GetTimerManager().SetTimer(TimerHandleAimSound, [&]() {
		bCanPlayAimSound = true; 
	}, 0.5f, false);
}

void AWeapon_Shotgun::GetEquipableObjectData_Basic_Implementation(FEquipableObjectData_Basic& EquipableObjectStruct) {
	EquipableObjectStruct = EquipableObjectData_Basic;
}

void AWeapon_Shotgun::SetEquipableObjectData_Basic_Implementation(FEquipableObjectData_Basic EquipableObjectStruct) {
	EquipableObjectData_Basic = EquipableObjectStruct;
}
#pragma endregion


#pragma region EquipableObject_Reload
bool AWeapon_Shotgun::Reload_Implementation() {
	if (Magazine.Num() < MagazineSize && EquipableObjectData_Reload.CandidateLoad) {
		Magazine.Push(EquipableObjectData_Reload.CandidateLoad);
		return true;
	}

	return false;
}

TSubclassOf<AActor> AWeapon_Shotgun::SetReadyLoadType_Implementation(int32 Slot) {
	if (Magazine.Num() >= MagazineSize) {
		return nullptr;
	}

	switch (Slot) {
	case 0: EquipableObjectData_Reload.CandidateLoad = EquipableObjectData_Reload.SlotFirstReloadProjectile; break;
	case 1: EquipableObjectData_Reload.CandidateLoad = EquipableObjectData_Reload.SlotSecondReloadProjectile; break;
	case 2: EquipableObjectData_Reload.CandidateLoad = EquipableObjectData_Reload.SlotThirdReloadProjectile; break;
	default: return false;
	}
	//If has susscessfully picked a valid candidate and the magazine its not full
	return EquipableObjectData_Reload.CandidateLoad;
}

bool AWeapon_Shotgun::CheckStatus_Implementation() {
	return EquipableObjectData_Reload.LoadReady == nullptr;
}

void AWeapon_Shotgun::SetReadyLoad_Implementation() {
	if (!Magazine.IsEmpty()) {
		EquipableObjectData_Reload.LoadReady = GetWorld()->SpawnActor(Magazine[0]);
		Magazine.RemoveAt(0);
		ProyectileBasic_Interface = Cast<IIProjectile_Basic>(EquipableObjectData_Reload.LoadReady);
		ProyectileBasic_Interface->Execute_GetProjectileData(EquipableObjectData_Reload.LoadReady, ProjectileData);
	}
}

void AWeapon_Shotgun::SetLoadsTypesToReload_Implementation(int32 Slot, TSubclassOf<AActor> Type) {
	switch (Slot) {
	case 0: EquipableObjectData_Reload.SlotFirstReloadProjectile = Type; break;
	case 1: EquipableObjectData_Reload.SlotSecondReloadProjectile = Type; break;
	case 2: EquipableObjectData_Reload.SlotThirdReloadProjectile = Type; break;
	default:break;
	}
}

void AWeapon_Shotgun::GetEquipableObjectData_Reload_Implementation(FEquipableObjectData_Reload& EquipableObjectStruct) {
	EquipableObjectStruct = EquipableObjectData_Reload;
}

void AWeapon_Shotgun::SetEquipableObjectData_Reload_Implementation(FEquipableObjectData_Reload EquipableObjectStruct) {
	EquipableObjectData_Reload = EquipableObjectStruct;
}
#pragma endregion

