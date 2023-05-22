// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../GeneralStructLibrary.h"
#include "../Character/ALSBaseCharacter.h"
#include "NecesityObject.generated.h"


class ATargetPoint;
class UAnimMontage;
class AAIController;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class ALSV4_CPP_API UNecesityObject : public UObject
{
	GENERATED_BODY()
public:
	UNecesityObject();

private:
	
	void PayOtherNecesities();

	void StartCalculateNewNecesities();

	UFUNCTION()
		void CalculateNewNecesities();

	UFUNCTION()
		void OnMoveCompleted();

	FVector GetClosestPoint(const FVector Point, const TArray<FVector>& PossiblesLocations) const;

public:
	FName GetJobName() const;

	FName GetNecesityName() const;

	float GetMinQuantityPerRegeneration() const;

	float GetMaxQuantityPerRegeneration() const;

	float GetMinTimeOnAction() const;

	float GetTimeRegeneration() const;

	TArray<FVector> GetNecesityLocations() const;

	UAnimMontage* GetMontageAction() const;

	TArray<FNecesityToPay> GetNecesitiesToPay() const;

	void Initialize(AAIController* NewController);
	
	UFUNCTION(BlueprintCallable)
	void StartSatisfyNecesity();
	UFUNCTION(BlueprintCallable)
	void StopSatisfyNecesity();

protected:
	/*Name of the job*/
	UPROPERTY(EditAnywhere, Category = "Necesity property")
		FName JobName;

	/*Name of the blackboard key to the necesity afected*/
	UPROPERTY(EditAnywhere, Category = "Necesity property")
		FName NecesityName;

	/*To not focus if its interrupted while is making a job*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Necesity property")
		FName CannotFocusTag;

	/*To let if can focus if its interrupted on its activity*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Necesity property")
		bool CannotFocusOtherLocation;

	/*Min threshold to random the quantity regenarion*/
	UPROPERTY(EditAnywhere, Category = "Necesity property")
		float MinQuantityPerRegeneration;
	/*Max threshold to random the quantity regenarion*/
	UPROPERTY(EditAnywhere, Category = "Necesity property")
		float MaxQuantityPerRegeneration;

	UPROPERTY(EditAnywhere, Category = "Necesity property")
		float MinQuantityAllowed;

	UPROPERTY(EditAnywhere, Category = "Necesity property")
		float MaxQuantityAllowed;

	/*Min threshold time to stay doing the action*/
	UPROPERTY(EditAnywhere, Category = "Necesity property", meta = (Units = "s"))
		float MinTimeOnAction;

	/*Time to next regeneration of the necesity*/
	UPROPERTY(EditAnywhere, Category = "Necesity property", meta = (Units = "s"))
		float TimeRegeneration;



	/*Location to make the action*/
	UPROPERTY(EditAnywhere, Category = "Necesity property")
		TArray<FVector> LocationsReference;

	/*Animation of the action*/
	UPROPERTY(EditAnywhere, Category = "Necesity property")
		TArray<UAnimMontage*> MontageActions;

	/*Other necesities to pay in exange to satisfy this*/
	UPROPERTY(EditAnywhere, Category = "Necesity property")
		TArray<FNecesityToPay> NecesitiesToPay;

	UPROPERTY(EditAnywhere, Category = "Necesity property")
		FVector LocationToFocus;




	UPROPERTY(EditAnywhere, Category = "Action property")
		float AcceptanceRadiusToMove;

	UPROPERTY(EditAnywhere, Category = "Action property")
		float BlendOutStop;


	UPROPERTY()
	AAIController* Owner;



private:
	UAnimMontage* MontageAction;
	FTimerHandle TimerHandleCalculateNewNecesities;
};
