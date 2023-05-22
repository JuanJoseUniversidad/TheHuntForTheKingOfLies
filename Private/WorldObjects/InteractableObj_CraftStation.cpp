// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldObjects/InteractableObj_CraftStation.h"

// Sets default values
AInteractableObj_CraftStation::AInteractableObj_CraftStation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene component"));
	SceneComponent->SetupAttachment(RootComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static component"));
	StaticMeshComponent->SetupAttachment(SceneComponent);

	PlaceActorSkeletalComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal component"));
	PlaceActorSkeletalComponent->SetupAttachment(SceneComponent);

	WeaponSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Static weapon component"));
	WeaponSkeletalMeshComponent->SetupAttachment(SceneComponent);
}

#pragma region WorldSystem_Basic
void AInteractableObj_CraftStation::ExecuteTimeEvent_Implementation(int32 Hour, EWeatherType Wheater) {
}

void AInteractableObj_CraftStation::GetUseSystemData_Implementation(FWorldSystemData_Basic& WorldSystemStruct) {
	WorldSystemStruct = WorldSystemData_Basic;
}

void AInteractableObj_CraftStation::SetUseSystemData_Implementation(FWorldSystemData_Basic WorldSystemStruct) {
	WorldSystemData_Basic = WorldSystemStruct;
}
#pragma endregion

bool AInteractableObj_CraftStation::Interact_Implementation(AActor* Interactor) {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Interact"));
	
	//Gets the craft knowledge of the player and sets it to the station
	ACharacter* BaseCharacter = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetCharacter();
	IICraftKnowledgeSystem* ICraftKnowledgeSystem_Interface = Cast<IICraftKnowledgeSystem>(BaseCharacter);
	ICraftKnowledgeSystem_Interface->Execute_GetCraftKnowledgeSystemData(BaseCharacter, CraftKnowledgeSystemDataRef);
	CraftingList = CraftKnowledgeSystemDataRef.CraftingList;

	//Creates the interface
	CreateCraftstationInterface();
	
	//Plays the audio
	if (InteractableObject_Basic.InteractSoundCue) {
		ACharacter* Character = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetCharacter();
		if (InteractableObject_Basic.bAttachedSound == true) {
			UGameplayStatics::SpawnSoundAttached(InteractableObject_Basic.InteractSoundCue, Character->GetMesh());
		}
		else {
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), InteractableObject_Basic.InteractSoundCue, Character->GetActorLocation());
		}
	}

	GetWorld()->GetTimerManager().SetTimer(TimerHandleEffects, [&]() {
		PlaceActorSkeletalComponent->SetHiddenInGame(false,true);
		WeaponSkeletalMeshComponent->SetHiddenInGame(false, true);

		ACharacter* Character = Cast<ACharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetCharacter());
		AALSCharacter* ALSCharacter = Cast<AALSCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetCharacter());
		
		if (ALSCharacter) {
			ALSCharacter->SetVisibleActorVisualsOnly(false);
		}else if(Character){
			Character->SetActorHiddenInGame(true);
		}

		if(Character){
			//Updates the mesh and the weapon
			PlaceActorSkeletalComponent->SetSkeletalMesh(Character->GetMesh()->SkeletalMesh);
			USkeletalMeshComponent* WeaponSkeletalMesh = Cast<USkeletalMeshComponent>(Character->FindComponentByClass<UChildActorComponent>()->GetChildActor()->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
			if (WeaponSkeletalMesh) {
				WeaponSkeletalMeshComponent->SetSkeletalMesh(WeaponSkeletalMesh->SkeletalMesh);
			}
		}

		//Execute placed actor montage
		if (PlaceActorMontage) {
			PlaceActorSkeletalComponent->GetAnimInstance()->Montage_Play(PlaceActorMontage);
		}

		//Change camera view
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTarget(this);
	}, TimeToApplyEffects, false);
	return true;
}

bool AInteractableObj_CraftStation::EndInteract_Implementation() {
	PlaceActorSkeletalComponent->SetHiddenInGame(true, true);
	PlaceActorSkeletalComponent->GetAnimInstance()->Montage_Stop(0);

	WeaponSkeletalMeshComponent->SetHiddenInGame(true, true);

	ACharacter* Character = Cast<ACharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetCharacter());
	AALSCharacter* ALSCharacter = Cast<AALSCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetCharacter());
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTarget(Character);
	
	if (ALSCharacter) {
		ALSCharacter->SetVisibleActorVisualsOnly(true);
	}
	else if (Character) {
		Character->SetActorHiddenInGame(false);
	}

	return true;
}

void AInteractableObj_CraftStation::GetInteractableData_Basic_Implementation(FInteractableData_Basic& InteractableStruct) {
	InteractableStruct = InteractableObject_Basic;
}

void AInteractableObj_CraftStation::SetInteractableData_Basic_Implementation(FInteractableData_Basic InteractableStruct) {
	InteractableObject_Basic = InteractableStruct;
}


// Called when the game starts or when spawned
void AInteractableObj_CraftStation::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractableObj_CraftStation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractableObj_CraftStation::CreateCraftstationInterface_Implementation() {
}