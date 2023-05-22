// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ALSCharacter.h"
#include "../Miscellaneous/NecesityObject.h"
#include "ALSNPCCharacter_NoHostile.generated.h"

//class UNecesityObject;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class ALSV4_CPP_API AALSNPCCharacter_NoHostile : public AALSCharacter
{
	GENERATED_BODY()
	
public:
	AALSNPCCharacter_NoHostile(const FObjectInitializer& ObjectInitializer);

	void ConstructNecesities();

	TMap<FName, UNecesityObject*> GetNecesityActions();

	FVector GetSleepLocation();

	FName GetSleepLocationKeyName();

protected:
	UPROPERTY(EditAnywhere, Category = "Routine Parameters")
		TArray<TSubclassOf<UNecesityObject>> ActionsToSatisfyNecesitiesClasses;

	UPROPERTY(EditAnywhere, Category = "Routine Parameters")
		FVector SleepLocation;

	UPROPERTY(VisibleAnywhere, Category = "Routine Parameters")
		FName SleepLocationKeyName;

	UPROPERTY(BlueprintReadOnly)
		TMap<FName,UNecesityObject*> NecesityActions;
};
