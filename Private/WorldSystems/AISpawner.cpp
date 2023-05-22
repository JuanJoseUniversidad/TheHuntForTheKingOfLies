// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldSystems/AISpawner.h"

// Sets default values
AAISpawner::AAISpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	RootComponent = Billboard;

	PlayerActivationRange = CreateDefaultSubobject<USphereComponent>(TEXT("PlayerActivationRange"));
	PlayerActivationRange->SetupAttachment(Billboard);
	PlayerActivationRange->OnComponentBeginOverlap.AddDynamic(this, &AAISpawner::OnOverlapBegin);

	PlayerDesactivationRange = CreateDefaultSubobject<USphereComponent>(TEXT("PlayerDeactivationRange"));
	PlayerDesactivationRange->SetupAttachment(Billboard);
	PlayerDesactivationRange->OnComponentEndOverlap.AddDynamic(this, &AAISpawner::OnOverlapEnd);

	AISpawnerData_Basic.SpawnedActors.Reserve(AISpawnerData_Basic.QuantityPerSpawn);

	TotalPullProbability = CalculateTotalProbability();
}


#pragma region WorldSystem_Basic

void AAISpawner::ExecuteTimeEvent_Implementation(int32 Hour, EWeatherType Wheater) {
	if (Hour >= WorldSystemData_Basic.HourActiveFrom && Hour <= WorldSystemData_Basic.HourActiveTo && (WorldSystemData_Basic.ValidWheaters.IsEmpty() || WorldSystemData_Basic.ValidWheaters.Contains(Wheater))) {
		AISpawnerData_Basic.bEnabledSpawner = true;
	}
	else {
		AISpawnerData_Basic.bEnabledSpawner = false;
	}
}

void AAISpawner::GetUseSystemData_Implementation(FWorldSystemData_Basic& WorldSystemStruct) {
	WorldSystemStruct = WorldSystemData_Basic;
}

void AAISpawner::SetUseSystemData_Implementation(FWorldSystemData_Basic WorldSystemStruct) {
	WorldSystemData_Basic = WorldSystemStruct;
}

#pragma endregion



#pragma region AISpawner_Basic

void AAISpawner::Spawn_Implementation() {
	if (bCanSpawn == true && AISpawnerData_Basic.bEnabledSpawner == true) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("yes"));
		for (int32 i = 0; i < AISpawnerData_Basic.SpawnedActors.Num(); i++){
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Iteration"));
			if (!IsValid(AISpawnerData_Basic.SpawnedActors[i])) {
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("found"));
				//Gets the position
				FVector FinalPosition;
				if (AISpawnerData_Basic.bRandomSpawn == true) {
					FinalPosition = UNavigationSystemV1::GetRandomReachablePointInRadius(GetWorld(), GetActorLocation(), AISpawnerData_Basic.RangeSpawn);
				}
				else {
					FinalPosition = AISpawnerData_Basic.SpawnPoint;
				}
				
				AISpawnerData_Basic.SpawnedActors[i] = UAIBlueprintHelperLibrary::SpawnAIFromClass(GetWorld(), PullAIClass(), nullptr, FinalPosition);

				//Calls next enemy to spawn with a timer, with this avoids lag spikes
				GetWorld()->GetTimerManager().SetTimer(TimerHandleSpawnNextEnemy, [&]() {
					Spawn_Implementation();
				}, DelayBetweenSpawnedAI, false);

				//Just the first found null actor, then exit the loop because the delayed function will found the next
				break;
			}
		}
	}
}

void AAISpawner::Despawn_Implementation() {
	for (int32 i = 0; i < AISpawnerData_Basic.SpawnedActors.Num(); i++) {
		if (IsValid(AISpawnerData_Basic.SpawnedActors[i])) {
			
			AISpawnerData_Basic.SpawnedActors[i]->Destroy();

			AISpawnerData_Basic.SpawnedActors[i] = nullptr;

			//Calls next enemy to spawn with a timer, with this avoids lag spikes
			GetWorld()->GetTimerManager().SetTimer(TimerHandleSpawnNextEnemy, [&]() {
				Despawn_Implementation();
			}, 0.2f, false);

			//Just the first found null actor, then exit the loop because the delayed function will found the next
			break;
		}

		//Clean all timers of this instance when all actors are destroyed
		if (i == AISpawnerData_Basic.SpawnedActors.Num() - 1) {
			GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
		}
	}
}


void AAISpawner::GetAISpawnerData_Basic_Implementation(FAISpawnerData_Basic& AISpawnerStruct) {
	AISpawnerStruct = AISpawnerData_Basic;
}

void AAISpawner::SetAISpawnerData_Basic_Implementation(FAISpawnerData_Basic AISpawnerStruct) {
	AISpawnerData_Basic = AISpawnerStruct;
}

#pragma endregion

#pragma region OtherMethods

int32 AAISpawner::CalculateTotalProbability() {
	int32 TotalProbability = 0;

	for(auto& Elem : AISpawnerData_Basic.SpawnableAIClasses) {
		TotalProbability += Elem.Value;
	}

	return TotalProbability;
}

TSubclassOf<APawn> AAISpawner::PullAIClass() {
	int32 RandomValue = FMath::RandRange(0, TotalPullProbability);
	TSubclassOf<APawn> DefaultClass;//In case there is no ticket pulled then just return the last class pulled (but this never happens but just in case)
	for(auto & Elem : AISpawnerData_Basic.SpawnableAIClasses) {
		if (TotalPullProbability <= Elem.Value) {
			return Elem.Key;
		}
		else {
			RandomValue -= Elem.Value;
			DefaultClass = Elem.Key;
		}
	}

	return DefaultClass;
}

#pragma endregion

// Called when the game starts or when spawned
void AAISpawner::BeginPlay()
{
	Super::BeginPlay();
	
	AISpawnerData_Basic.SpawnedActors.Init(nullptr, AISpawnerData_Basic.QuantityPerSpawn);
}

// Called every frame
void AAISpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAISpawner::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (FVector::Dist(OtherActor->GetActorLocation(), GetActorLocation()) <= PlayerActivationRange->GetScaledSphereRadius()+(PlayerActivationRange->GetScaledSphereRadius()*0.2f) && bActivated == false) {
		bCanSpawn = true;
		bActivated = true;
		//Calls instantly the next wave
		Spawn_Implementation();

		//Start timer to call next wave of enemies
		GetWorld()->GetTimerManager().SetTimer(TimerHandleSpawnNextWave, [&]() {
			Spawn_Implementation();
		}, AISpawnerData_Basic.SpawnWaveDelay, true);
	}
}

void AAISpawner::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	//If its in far away from the center point to avoid to trigger this event when its ragdoll
	if (FVector::Dist(OtherActor->GetActorLocation(), GetActorLocation()) >= PlayerDesactivationRange->GetScaledSphereRadius()-(PlayerDesactivationRange->GetScaledSphereRadius()*0.2f)) {
		bCanSpawn = false;
		bActivated = false;
		//Calls instantly the despawn
		Despawn_Implementation();
	}
}

