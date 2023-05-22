// Fill out your copyright notice in the Description page of Project Settings.


#include "InstantHealingObject.h"

// Sets default values
AInstantHealingObject::AInstantHealingObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

bool AInstantHealingObject::Consume_Implementation(AActor* Actor) {
	ACharacter* Character = Cast<ACharacter>(Actor);
	UActorComponent* HealthComponent = nullptr;
	IIHealthComponent* HealthComponent_Interface = nullptr;

	if (!Character->GetComponentsByInterface(UIHealthComponent::StaticClass()).IsEmpty()) {
		HealthComponent = Character->GetComponentsByInterface(UIHealthComponent::StaticClass())[0];
		HealthComponent_Interface = Cast<IIHealthComponent>(HealthComponent);
	}

	if (HealthComponent_Interface) {
		//Cures the player 
		HealthComponent_Interface->Execute_AddHealth(HealthComponent, CureHeal);

		//Spawn effect attached or not
		if (ConsumableObjectData_Basic.bAttachedEffect == true) {
			UNiagaraFunctionLibrary::SpawnSystemAttached(ConsumableObjectData_Basic.ConsumableEffect, Character->GetMesh(), "none", FVector(0, 0, 0), FRotator(0, 0, 0), EAttachLocation::SnapToTarget, true);
		}
		else {
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ConsumableObjectData_Basic.ConsumableEffect, Character->GetActorLocation());
		}

		//Spawn sound attached or not
		if (ConsumableObjectData_Basic.bAttachedSound == true) {
			UGameplayStatics::SpawnSoundAttached(ConsumableObjectData_Basic.ConsumableSoundCue, Character->GetMesh());
		}
		else {
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ConsumableObjectData_Basic.ConsumableSoundCue, Character->GetActorLocation());
		}

		return true;
	}

	return false;
}

void AInstantHealingObject::GetConsumableObjectData_Basic_Implementation(FConsumableObjectData_Basic& ConsumableObjectStruct) {
	ConsumableObjectStruct = ConsumableObjectData_Basic;
}

void AInstantHealingObject::SetConsumableObjectData_Basic_Implementation(FConsumableObjectData_Basic ConsumableObjectStruct) {
	ConsumableObjectData_Basic = ConsumableObjectStruct;
}

// Called when the game starts or when spawned
void AInstantHealingObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInstantHealingObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

