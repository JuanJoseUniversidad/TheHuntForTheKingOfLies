// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../GeneralStructLibrary.h"
#include "../Interfaces/IAISpawner_Basic.h"
#include "../Interfaces/IWorldSystem_Basic.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/BillboardComponent.h"
#include "AISpawner.generated.h"

UCLASS()
class ALSV4_CPP_API AAISpawner : public AActor, public IIWorldSystem_Basic, public IIAISpawner_Basic
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAISpawner();

	//IIWorldSystem_Basic
	virtual void ExecuteTimeEvent_Implementation(int32 Hour, EWeatherType Wheater) override;
	virtual void GetUseSystemData_Implementation(FWorldSystemData_Basic& WorldSystemStruct) override;
	virtual void SetUseSystemData_Implementation(FWorldSystemData_Basic WorldSystemStruct) override;

	//IIAISpawner_Basic
	virtual void Spawn_Implementation() override;
	virtual void Despawn_Implementation() override;
	virtual void GetAISpawnerData_Basic_Implementation(FAISpawnerData_Basic& AISpawnerStruct) override;
	virtual void SetAISpawnerData_Basic_Implementation(FAISpawnerData_Basic AISpawnerStruct) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the end starts or when destroyed
	//virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	/**
	 * Calculate the total probability of the pool AI's
	 * @return The total probability of the pool
	 */
	int32 CalculateTotalProbability();

	/**
	 * Pull a AI class to spawn
	 * @return The AI class
	 */
	TSubclassOf<APawn> PullAIClass();

public:
	//Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
		USphereComponent* PlayerActivationRange;//Range where the spawner is activated

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
		USphereComponent* PlayerDesactivationRange;//Range where the spawner is activated

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
		UBillboardComponent* Billboard;

	//Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		FAISpawnerData_Basic AISpawnerData_Basic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		FWorldSystemData_Basic WorldSystemData_Basic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Properties")
		float DelayBetweenSpawnedAI = 3.0f;

private:
	bool bCanSpawn = true;
	bool bActivated = false;
	int32 TotalPullProbability = 0;

	FTimerHandle TimerHandleSpawnNextEnemy;
	FTimerHandle TimerHandleSpawnNextWave;
};
