#pragma once

#include "CoreMinimal.h"

#include "Animation/AnimMontage.h"
#include "Engine/DataTable.h"
#include "UObject/ObjectPtr.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Sound/SoundCue.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GeneralEnumLibrary.h"
#include "Library/ALSCharacterEnumLibrary.h"
#include "GeneralStructLibrary.generated.h"

USTRUCT(BlueprintType)
struct FInventoryObjectData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties")
		TSubclassOf<AActor> ObjectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties")
		EObjectCategory ObjectCategory = EObjectCategory::Other;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties", meta = (AllowedClasses = "Texture,MaterialInstance", ExactClass = false))
		UObject* ObjectIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties")
		FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties")
		FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties")
		int32 Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		USoundCue* SoundCue;
};

USTRUCT(BlueprintType)
struct FWorldSystemData_Basic {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		int32 HourActiveFrom = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		int32 HourActiveTo = 23;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		TArray<EWeatherType> ValidWheaters;
};

USTRUCT(BlueprintType)
struct FEquipableObjectData_Basic {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Properties")
		EALSOverlayState State = EALSOverlayState::Default;

	UPROPERTY(EditAnywhere, Category = "Properties")
		UAnimMontage* EquipMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Properties")
		UAnimMontage* UnequipMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties")
		TArray<UAnimMontage*> UseMontages;
};

USTRUCT(BlueprintType)
struct FEquipableObjectData_Reload {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Properties")
		UAnimMontage* ReloadMontage;

	UPROPERTY(EditAnywhere, Category = "Properties")
		UAnimMontage* ReloadSecondaryMontage;

	UPROPERTY(BlueprintReadOnly)
		AActor* LoadReady;

	UPROPERTY(BlueprintReadOnly)
		TSubclassOf<AActor> CandidateLoad;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Properties", meta = (MustImplement = "IProjectile_Basic"))
		TSubclassOf<AActor> SlotFirstReloadProjectile;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties", meta = (MustImplement = "IProjectile_Basic"))
		TSubclassOf<AActor> SlotSecondReloadProjectile;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties", meta = (MustImplement = "IProjectile_Basic"))
		TSubclassOf<AActor> SlotThirdReloadProjectile;
};

USTRUCT(BlueprintType)
struct FEquipSystemData {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		FName SocketNameEquipment = "EquippedObject";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		FName SocketNameHandEquip = "HandSocket";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		bool bCanEquip = true;

	bool bInEquip = false;
	bool bEquipped = false;
};

USTRUCT(BlueprintType)
struct FAimSystemData {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		bool bCanAim = true;
};

USTRUCT(BlueprintType)
struct FUseSystemData {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		bool bCanUse = false;

	bool bInUse = false;
};

USTRUCT(BlueprintType)
struct FReloadSystemData {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Properties")
		EALSOverlayState ReloadModeState = EALSOverlayState::Reload;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		FName SocketNameReload = "Hand_LSocket";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		bool bCanReloadMode = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		bool bCanReload = true;

	bool bInReloadMode = false;
};



USTRUCT(BlueprintType)
struct FProjectileData {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Properties")
		int32 NumberOfShoots = 1;

	UPROPERTY(EditAnywhere, Category = "Properties")
		float Range = 50000.0f;

	UPROPERTY(EditAnywhere, Category = "Properties")
		float Spread = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Properties")
		float Damage = 5.0f;

	//The impulse that the bones of the body will recive
	UPROPERTY(EditAnywhere, Category = "Properties")
		double BoneImpulse = 5000.0;

	//The impulse per NumberOfShoots that the integrity of the character will be recive
	UPROPERTY(EditAnywhere, Category = "Properties")
		double KnockbackImpulsePerShoot = 5000.0;

	UPROPERTY(EditAnywhere, Category = "Properties")
		UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, Category = "Properties")
		TObjectPtr<UDataTable> BallisticFXDataTable = nullptr;
};

USTRUCT(BlueprintType)
struct FConsumableObjectData_Basic {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		bool bAttachedEffect = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		UNiagaraSystem* ConsumableEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		bool bAttachedSound = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		USoundCue* ConsumableSoundCue;

	UPROPERTY(EditAnywhere, Category = "Properties")
		UAnimMontage* ConsumableAnimation;
};

USTRUCT(BlueprintType)
struct FPickableObjectData_Basic {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		TSubclassOf<AActor> ObjectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		bool bDestroyOnPick = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		float TimeToRegenerate = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		bool bAttachedSound = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		USoundCue* PickSoundCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		FInventoryObjectData ObjectData;

	UPROPERTY(EditAnywhere, Category = "Properties")
		UAnimMontage* PickAnimation;
};

USTRUCT(BlueprintType)
struct FInteractableData_Basic {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		FText Name = FText::FromString("???");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		EInteractionCategory InteractionCategory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		bool bDisableOnInteract = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		float TimeToRenable = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		bool bAttachedSound = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		USoundCue* InteractSoundCue;

	UPROPERTY(EditAnywhere, Category = "Properties")
		UAnimMontage* InteractAnimation;
};

USTRUCT(BlueprintType)
struct FCrafteableObjectData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties")
		FInventoryObjectData ObjectToCraft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties")
		TArray<FInventoryObjectData> RequiredObjects;
};


USTRUCT(BlueprintType)
struct FDashComponentData {
	GENERATED_BODY()

public:
	//When is stand
	UPROPERTY(EditAnywhere, Category = "Properties | Stand")
		float DashPower = 1500.0f;

	UPROPERTY(EditAnywhere, Category = "Properties | Stand")
		float DelayReuseDash = 0.8f;

	UPROPERTY(EditAnywhere, Category = "Properties | Stand")
		float DelayEnableAnimations = 0.3f; //Time to enable again the animation bp, this time is based on the dashPower(more power more distance)

	UPROPERTY(EditAnywhere, Category = "Properties | Stand")
		float DelayVisiblePlayer = 0.5f; //Time to enable again the visibility of the player, this time is based on the delay of the reset of animBP

	//When is crouch
	UPROPERTY(EditAnywhere, Category = "Properties | Crouch")
		float DashPowerCrouch = 2500.0f;

	UPROPERTY(EditAnywhere, Category = "Properties | Crouch")
		float DelayReuseDashCrouch = 0.9f;

	UPROPERTY(EditAnywhere, Category = "Properties | Crouch")
		float DelayEnableAnimationsCrouch = 0.1f; //Time to enable again the animation bp, this time is based on the dashPower(more power more distance)

	UPROPERTY(EditAnywhere, Category = "Properties | Crouch")
		float DelayVisiblePlayerCrouch = 0.4f; //Time to enable again the visibility of the player, this time is based on the delay of the reset of animBP

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | AI | BlackBoard")
		float CoolDownDashAI = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | AI | BlackBoard")
		float DistanceToDashAI = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		bool bCanDash = true;

	//Duration of the action, specially used for define the duration of the tasks in the AI behaviour tree
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		float DashDuration = 0.7f;

	bool bInDash = false;

	TArray<TFunction<void()>> StartDashFunctions;
	TArray<TFunction<void()>> EndDashFunctions;
};

USTRUCT(BlueprintType)
struct FConsumeComponentData {
	GENERATED_BODY()

public:
	/*Use consumable object system*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		bool bCanConsume = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		FName SocketNameConsumable = "Hand_LSocket_Consumable";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (MustImplement = "IConsumableObject_Basic"))
		TSubclassOf<AActor> ConsumableClass;

	/*Is in use the consumable object*/
	bool bInConsume = false;

	AActor* ActorConsumable;
};

USTRUCT(BlueprintType)
struct FInventoryComponentData {
	GENERATED_BODY()

public:
	/* Inventory */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties")
		TMap<TSubclassOf<AActor>, FInventoryObjectData> Inventory;
};

USTRUCT(BlueprintType)
struct FPickComponentData {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		bool bCanPick = true;

	/*Is in pick the object*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		bool bInPick = false;

	TArray<AActor*> PickableCandidates;
	
	UPROPERTY(BlueprintReadOnly, Category = "Properties")
		AActor* ObjectToPick;
};

USTRUCT(BlueprintType)
struct FInteractComponentData {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		bool bCanInteract = true;

	/*Is in interacting with the object*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		bool bInInteract = false;

	UPROPERTY(BlueprintReadOnly, Category = "Properties")
		AActor* ObjectToInteract;
};

USTRUCT(BlueprintType)
struct FDialogueComponentData {
	GENERATED_BODY()

public:
	//Name's owner of the component
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties")
		FName Name = "???";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties")
		FName Nickname = "";

	//Reference to the actor who triggers the dialogue
	UPROPERTY(BlueprintReadWrite, Category = "Properties")
		AActor* InteractorReference;

	//List of unique visited dialogs by the interactor
	UPROPERTY(BlueprintReadWrite, Category = "Properties")
		TArray<int32> UniqueInteractorsVisitedResponses;

	//List of unique visited dialogs by the owner
	UPROPERTY(BlueprintReadWrite, Category = "Properties")
		TArray<int32> UniqueOwnersVisitedResponses;

	//List of already visited dialogs by the interactor
	UPROPERTY(BlueprintReadWrite, Category = "Properties")
		TArray<int32> InteractorsVisitedResponses;
};

USTRUCT(BlueprintType)
struct FHealthComponentData {
	GENERATED_BODY()

public:
	/* Life */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Life")
		float MaxLife = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Life")
		float Life = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Life")
		bool bIsDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Life")
		bool bIsInvulnerable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Life")
		bool bDisableInputCharacter = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Life")
		bool bDestroyOnDeath = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Life")
		float TimeDestroyOnDeath = 10.0f;


	/* Fall damage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Fall damage")
		float TimeFallDamageRestart = 0.2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Fall damage")
		float FallDamageThreshold = 450.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Fall damage")
		float FallDamageMultiplicator = 20;


	/* Hit damage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Hit damage")
		float AnimationWeight = 1.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Hit damage")
		float ImpulseBoneHitDivider = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Hit damage")
		float RecoveryTimeInterpolation = 5;//Less time means it will take much longer

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components | Hit damage")
		UPhysicalAnimationComponent* PhysicsAnimComponent;

	bool bStartRecoveryFromHit = false;


	/* Knockback */
	//Force of how much knockback can hold until enter on ragdollmode, if ist -1 the knockback is inexistence
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health | Properties | Knockback", meta = (Units = "newtons"))
		float KnockbackToleranceThreshold = 3000.0f;

	//How much time take to recover from the knockback, higher value more knockback can accumulate over time
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health | Properties | Knockback", meta = (Units = "seconds"))
		float KnockbackTimeRecover = 0.03f;

	bool bIsStartingRecoverFromKnockback = false;
};



USTRUCT(BlueprintType)
struct FAISpawnerData_Basic {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		bool bEnabledSpawner = true;

	//The key represent the class to spawn and the in32 represents the number of tickets that have the ai to spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		TMap<TSubclassOf<APawn>, int32> SpawnableAIClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		int32 QuantityPerSpawn = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Units = "s"), Category = "Properties")
		float SpawnWaveDelay = 30.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		bool bRandomSpawn = true;

	//Specific point to spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bRandomSpawn == false"), Category = "Properties")
		FVector SpawnPoint;

	//The range of the spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bRandomSpawn == true"), Category = "Properties")
		float RangeSpawn;

	TArray<AActor*> SpawnedActors;
};

USTRUCT(BlueprintType)
struct FGlobalSoundsManagerSystemData_Basic {
	GENERATED_BODY()

public:
	//List of sounds to be spawned at the beggining and its base event by default
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		TMap<USoundBase*, FName> Sounds;

	//List of audio components which contains the spawned sound
	UPROPERTY(BlueprintReadOnly, Category = "Properties")
		TArray<UAudioComponent*> AudiosComponent;

	//List of bools which indicates if a sound is on situational state (which means that the actual sound its executing a event just for a specific event until exit)
	TArray<bool> bSoundsEnterSituationalMode;

};

//!!It must be here to avoid errors because it needs to declare FCrafteableObjectData before
USTRUCT(BlueprintType)
struct FCraftKnowledgeSystemData {
	GENERATED_BODY()

public:
	//The Key is the crafteable item and the value are the required objects
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		TArray<FCrafteableObjectData> CraftingList;
};


USTRUCT(BlueprintType)
struct FNecesityToPay {
	GENERATED_BODY()

public:
	//Blackboard key of the necesity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		FName BlackBoardKey;

	//Cost of the necesity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		float NecesityCost;
};