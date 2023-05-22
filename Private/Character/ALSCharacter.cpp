// Copyright:       Copyright (C) 2022 Doğa Can Yanıkoğlu
// Source Code:     https://github.com/dyanikoglu/ALS-Community


#include "Character/ALSCharacter.h"

#include "Engine/StaticMesh.h"
#include "AI/ALSAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"

#define DASH_COMPONENT_CONDITION (DashComponent == nullptr || DashComponent->DashComponentData.bInDash == false)
#define CONSUME_COMPONENT_CONDITION (ConsumeComponent == nullptr || ConsumeComponent->ConsumeComponentData.bInConsume == false)
#define PICK_COMPONENT_CONDITION (PickComponent == nullptr || PickComponent->PickComponentData.bInPick == false)

//-- Edited --//
AALSCharacter::AALSCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	HeldObjectRoot = CreateDefaultSubobject<USceneComponent>(TEXT("HeldObjectRoot"));
	HeldObjectRoot->SetupAttachment(GetMesh());

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(HeldObjectRoot);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(HeldObjectRoot);

	PhysicsAnchor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Physics anchor"));
	PhysicsAnchor->SetupAttachment(GetMesh(), EquipSystemData.SocketNameEquipment);

	PhysicsContraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Physics constraint"));
	PhysicsContraint->SetupAttachment(PhysicsAnchor);

	ObjectAnchor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Equipped object anchor"));
	ObjectAnchor->SetupAttachment(GetMesh(), EquipSystemData.SocketNameEquipment);

	EquippedPivot = CreateDefaultSubobject<USceneComponent>(TEXT("Equipped object"));
	EquippedPivot->SetupAttachment(ObjectAnchor);

	EquippedActorComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("Equipped actor"));
	EquippedActorComponent->SetupAttachment(EquippedPivot);
	


	ReloadShellAnchor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shell anchor"));
	ReloadShellAnchor->SetupAttachment(GetMesh(), ReloadSystemData.SocketNameReload);

	StimuliSourceComp = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimuli source component"));

	AIControllerClass = AALSAIController::StaticClass();

	AffiliationTagName = "Affiliation.Villager";
}

void AALSCharacter::ClearHeldObject()
{
	StaticMesh->SetStaticMesh(nullptr);
	SkeletalMesh->SetSkeletalMesh(nullptr);
	SkeletalMesh->SetAnimInstanceClass(nullptr);
}

void AALSCharacter::AttachToHand(UStaticMesh* NewStaticMesh, USkeletalMesh* NewSkeletalMesh, UClass* NewAnimClass,
                                 bool bLeftHand, FVector Offset)
{
	ClearHeldObject();

	if (IsValid(NewStaticMesh))
	{
		StaticMesh->SetStaticMesh(NewStaticMesh);
	}
	else if (IsValid(NewSkeletalMesh))
	{
		SkeletalMesh->SetSkeletalMesh(NewSkeletalMesh);
		if (IsValid(NewAnimClass))
		{
			SkeletalMesh->SetAnimInstanceClass(NewAnimClass);
		}
	}

	FName AttachBone;
	if (bLeftHand)
	{
		AttachBone = TEXT("VB LHS_ik_hand_gun");
	}
	else
	{
		AttachBone = TEXT("VB RHS_ik_hand_gun");
	}

	HeldObjectRoot->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, EquipSystemData.SocketNameHandEquip);//AttachBone
	HeldObjectRoot->SetRelativeLocation(Offset);
}

void AALSCharacter::RagdollStart()
{
	SetOverlayState(EALSOverlayState::Default);
	ClearHeldObject();
	Super::RagdollStart();

	ReloadShellAnchor->SetStaticMesh(nullptr);

	if (ConsumeComponent) {
		ConsumeComponent->ConsumableAnchor->SetStaticMesh(nullptr);
	}
}

//-- Edited --//
void AALSCharacter::RagdollEnd()
{
	double BoneTotalVelocity = UKismetMathLibrary::Abs(GetMesh()->GetPhysicsLinearVelocity(BoneNameRagdollVelocity).X) + UKismetMathLibrary::Abs(GetMesh()->GetPhysicsLinearVelocity(BoneNameRagdollVelocity).Y) + UKismetMathLibrary::Abs(GetMesh()->GetPhysicsLinearVelocity(BoneNameRagdollVelocity).Z);
	if (BoneTotalVelocity <= GetUpFromRagdollThreshold) {
		Super::RagdollEnd();
		UpdateHeldObject();
		//Resets all actions
		UseSystemData.bInUse = false;
		ReloadSystemData.bInReloadMode = false;
		ReloadSystemData.bCanReload = true;
		EquipSystemData.bInEquip = false;

		if (ConsumeComponent) {
			ConsumeComponent->ConsumeComponentData.bInConsume = false;
		}

		if (PickComponent) {
			PickComponent->PickComponentData.bInPick = false;
		}

		if (DashComponent) {
			DashComponent->DashComponentData.bInDash = false;
		}

		if (InteractComponent) {
			InteractComponent->InteractComponentData.bInInteract = false;
		}

		//If has anything equipped then set the corresponding overlay
		if (EquipSystemData.bEquipped == true) {
			SetOverlayState(EquipableObjectDataBase.State);
		}

		//Reload the chamber bullet in case the ragdoll interrupts the shoot(checks forst if the characters has an equippable object and if its implements the reload interface)
		if (EquippedActorComponent->GetChildActor() && EquippableObjectReload_Interface && EquippableObjectReload_Interface->Execute_CheckStatus(EquippedActorComponent->GetChildActor()) == true) {
			EquippableObjectReload_Interface->Execute_SetReadyLoad(EquippedActorComponent->GetChildActor());
		}
	}
}

ECollisionChannel AALSCharacter::GetThirdPersonTraceParams(FVector& TraceOrigin, float& TraceRadius)
{
	const FName CameraSocketName = bRightShoulder ? TEXT("TP_CameraTrace_R") : TEXT("TP_CameraTrace_L");
	TraceOrigin = GetMesh()->GetSocketLocation(CameraSocketName);
	TraceRadius = 15.0f;
	return ECC_Camera;
}

FTransform AALSCharacter::GetThirdPersonPivotTarget()
{
	return FTransform(GetActorRotation(),
	                  (GetMesh()->GetSocketLocation(TEXT("Head")) + GetMesh()->GetSocketLocation(TEXT("root"))) / 2.0f,
	                  FVector::OneVector);
}

FVector AALSCharacter::GetFirstPersonCameraTarget()
{
	return GetMesh()->GetSocketLocation(TEXT("FP_Camera"));
}

void AALSCharacter::OnOverlayStateChanged(EALSOverlayState PreviousState)
{
	Super::OnOverlayStateChanged(PreviousState);
	UpdateHeldObject();
}

//-- Edited --//
void AALSCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) {
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
	
	//Checks if its in air and if its on reload mode, then exits from the mode
	if (GetMovementState() == EALSMovementState::InAir && EquipSystemData.bEquipped == true && ReloadSystemData.bInReloadMode == true) {
		ReloadMode_Implementation(false);
	}
}

void AALSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateHeldObjectAnimations();

	//--
	if (DashComponent && DashComponent->DashComponentData.bInDash == true && GetMovementState() == EALSMovementState::Ragdoll) {
		DashComponent->DashComponentData.bInDash = false;
		DashComponent->EndDash_Implementation(0.3,0.1,0.2);
	}
}

//-- Edited --//
void AALSCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Equip new object
	SetNewEquippableObject();

	//Updates the state overlay to update the scene component and dont show the rifle on the floor when the first time its equipped
	SetOverlayState(EALSOverlayState::Rifle);
	SetOverlayState(DefaultState);

	//equippableObjectInterface = nullptr;
	EquippableObjectBasic_Interface = nullptr;
	EquippableObjectReload_Interface = nullptr;

	//Try to get components and sets parameters

	//Health component
	ALSHealthComponent = Cast<UALSHealth_Component>(GetComponentByClass(UALSHealth_Component::StaticClass()));

	//Dash component
	DashComponent = Cast<UDash_Component>(GetComponentByClass(UDash_Component::StaticClass()));
	if (DashComponent) {
		//Sets the lambda expressions used on StartDash and EndDash
		DashComponent->DashComponentData.StartDashFunctions = {
						[&]() {
							SetVisibleActorVisualsOnly(DashComponent->GetMakeVisibleCharacter());
							//SetActorHiddenInGame(true);
						}
		};

		DashComponent->DashComponentData.EndDashFunctions = {
						[&]() {
							GetCharacterMovement()->Velocity.Z = DashComponent->VelocityZPreDash;
							bSimGravityDisabled = false;
							GetMesh()->bPauseAnims = false;
						},
						[&]() {
							//Can change movement state
							bCanChangeMovementState = true;

							if (ALSHealthComponent) {
								//Sets the invulnerability to false
								ALSHealthComponent->HealthComponentData.bIsInvulnerable = false;
							}

							//Can perform actions again
							bBlockActions = false;

							SetVisibleActorVisualsOnly(true);
							//SetActorHiddenInGame(false);
						}
		};
	}

	//Consume component
	ConsumeComponent = Cast<UConsume_Component>(GetComponentByClass(UConsume_Component::StaticClass()));

	//Inventory component
	InventoryComponent = Cast<UInventory_Component>(GetComponentByClass(UInventory_Component::StaticClass()));

	//Pick component
	PickComponent = Cast<UPick_Component>(GetComponentByClass(UPick_Component::StaticClass()));

	//Interact component
	InteractComponent = Cast<UInteract_Component>(GetComponentByClass(UInteract_Component::StaticClass()));

	UpdateHeldObject();
}





/*-= EQUIP SYSTEM =-*/
#pragma region EquipSystem
void AALSCharacter::Equip_Implementation() {
	EquipSystemData.bEquipped = true;
	EquippedActorComponent->AttachToComponent(HeldObjectRoot, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::SnapToTarget, false) , EquipSystemData.SocketNameHandEquip);
	SetNewEquippableObject();
}

void AALSCharacter::Unequip_Implementation() {
	EquipSystemData.bEquipped = false;
	EquippedActorComponent->AttachToComponent(EquippedPivot, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::SnapToTarget, false), EquipSystemData.SocketNameEquipment);
	
	//equippableObjectInterface = nullptr;

	EquippableObjectBasic_Interface = nullptr;
	EquippableObjectReload_Interface = nullptr;
}

void AALSCharacter::GetEquipSystemData_Implementation(FEquipSystemData& EquipSystemStruct) {
	EquipSystemStruct = EquipSystemData;
}

void AALSCharacter::SetEquipSystemData_Implementation(FEquipSystemData EquipSystemStruct) {
	EquipSystemData = EquipSystemStruct;
}

void AALSCharacter::EquipAction_Implementation(bool bValue) {
	if (EquipSystemData.bCanEquip == true && bValue == true && CanPerformAction(EALSAction::Equip) == true) {
		if (EquipSystemData.bEquipped == false && EquipSystemData.bInEquip == false && EquipableObjectDataBase.EquipMontage) {
			EquipSystemData.bInEquip = true;
			GetMesh()->GetAnimInstance()->Montage_Play(EquipableObjectDataBase.EquipMontage, 1.0f);
			//Removes the affiliation tag to advertise that its a potential threat to non hostile npcs
			Tags.Remove(AffiliationTagName);

		}
		else if (EquipSystemData.bInEquip == false && EquipableObjectDataBase.UnequipMontage) {
			EquipSystemData.bInEquip = true;
			GetMesh()->GetAnimInstance()->Montage_Play(EquipableObjectDataBase.UnequipMontage, 1.0f);
			//Add the affiliation tag to advertise that its a not already a potential threat to non hostile npcs
			Tags.Add(AffiliationTagName);
		}
	}

	//Calls the BP implementation layer
	EquipBP(bValue);

	//bInEquip = true -> inside the montage at the end
}

void AALSCharacter::EquipBP_Implementation(bool bValue) {
}

void AALSCharacter::SetNewEquippableObject(TSubclassOf<AActor> NewEquippableObject){
	if (NewEquippableObject) {
		EquippedActorComponent->SetChildActorClass(NewEquippableObject);
	}
	
	if (EquippedActorComponent->GetChildActor()) {
		//Set the equipable object owner
		EquippedActorComponent->GetChildActor()->SetOwner(this);


		//Gets the basic interface of the equipped object if exsist
		EquippableObjectBasic_Interface = Cast<IIEquipableObject_Basic>(EquippedActorComponent->GetChildActor());
		//Gets the basic data struct of the equipped object
		if (EquippableObjectBasic_Interface) {
			EquippableObjectBasic_Interface->Execute_GetEquipableObjectData_Basic(EquippedActorComponent->GetChildActor(), EquipableObjectDataBase);
		}

		//Try to get the reload interface of the equipped object if exist
		EquippableObjectReload_Interface = Cast<IIEquipableObject_Reload>(EquippedActorComponent->GetChildActor());
		//Gets the reload data struct of the equipped object
		if (EquippableObjectReload_Interface) {
			EquippableObjectReload_Interface->Execute_GetEquipableObjectData_Reload(EquippedActorComponent->GetChildActor(), EquippableObjectDataReload);
		}
	}
}
#pragma endregion 

/*-= AIM SYSTEM =-*/
#pragma region AimSystem
void AALSCharacter::Aim_Implementation(bool bAiming) {
	//If its true interpolates the opacity of the crosshead of the HUD widget to 1 if not to 0
	UpdateCrosshairOpacity(bAiming);

	//If the has a equippable object
	if (EquippableObjectBasic_Interface) {
		//Aim sound of the equipped object
		if (bAiming == true && CanPerformAction(EALSAction::Aim) == true) {
			EquippableObjectBasic_Interface->Execute_PlayReadySound(EquippedActorComponent->GetChildActor());
		}
		else {
			EquippableObjectBasic_Interface->Execute_StopReadySound(EquippedActorComponent->GetChildActor());
		}

		//Enable/Disable can use equipped object with delay to avoid use it without aim
		if (bAiming == true) {
			EnableCanUse(0.1f);
		}
		else {
			DisableCanUse();
		}
	}
}

void AALSCharacter::GetAimSystemData_Implementation(FAimSystemData& AimSystemStruct) {
	AimSystemStruct = AimSystemData;
}

void AALSCharacter::SetAimSystemData_Implementation(FAimSystemData AimSystemStruct) {
	AimSystemData = AimSystemStruct;
}

void AALSCharacter::UpdateCrosshairOpacity_Implementation(bool bValue) {
}
#pragma endregion

/*-= USE SYSTEM =-*/
#pragma region UseSystem
void AALSCharacter::Use_Implementation() {
	if (EquippableObjectBasic_Interface->Execute_TryToUse(EquippedActorComponent->GetChildActor(), FMath::RandRange(0, EquipableObjectDataBase.UseMontages.Num() - 1)) >= 0.0f) {
		UseSystemData.bInUse = true;
	}
	else {
		UseSystemData.bInUse = false;
	}
	//bInUse = false; on the montage when finish the animation
	//if (UseSystemData.bInUse) {
	//	GetMesh()->GetAnimInstance()->Montage_Play(EquipableObjectDataBase.UseMontage, 1.0f);
	//}
}

void AALSCharacter::GetUseSystemData_Implementation(FUseSystemData& UseSystemStruct) {
	UseSystemStruct = UseSystemData;
}

void AALSCharacter::SetUseSystemData_Implementation(FUseSystemData UseSystemStruct) {
	UseSystemData = UseSystemStruct;
}

void AALSCharacter::UseAction_Implementation(bool bValue) {
	if (UseSystemData.bCanUse == true && bValue == true && CanPerformAction(EALSAction::Use) == true) {
		Use_Implementation();

		//Calls the BP implementation layer
		UseBP(bValue);
	}
}

void AALSCharacter::UseBP_Implementation(bool bValue) {
}

void AALSCharacter::EnableCanUse(float delay) {
	GetWorld()->GetTimerManager().SetTimer(TimerHandleUse, [&]() {
		UseSystemData.bCanUse = true; 
	}, delay, false);
}

void AALSCharacter::DisableCanUse() {
	GetWorld()->GetTimerManager().ClearTimer(TimerHandleUse);
	UseSystemData.bCanUse = false;
}
#pragma endregion

/*-= RELOAD SYSTEM =-*/
#pragma region ReloadSystem
void AALSCharacter::ReloadMode_Implementation(bool bReloadMode) {
	//Here checks if wants to enter on reload state and its no in ragdoll, falling, rolling state, in those cases select the corresponding state if there is equipped something or not
	if (bReloadMode == true && CanPerformAction(EALSAction::ReloadMode) == true) {
		if (EquipSystemData.bEquipped == true && ReloadSystemData.bCanReloadMode == true) {
			SetOverlayState(ReloadSystemData.ReloadModeState);
			//If its sprinting set the state to running
			if (GetGait() == EALSGait::Sprinting) {
				SetDesiredGait(EALSGait::Running);
			}
			ReloadSystemData.bInReloadMode = true;
			EquipSystemData.bCanEquip = false;

			//Calls the BP implementation layer (only true value)
			EnterReloadModeBP(bReloadMode);
		}
	}
	else if(ReloadSystemData.bCanReload == true && EquipSystemData.bInEquip == false) {
		if (EquipSystemData.bEquipped == true) {
			SetOverlayState(EquipableObjectDataBase.State);
			//This is for avoid to walk or trot when enter on one of those states or enter on walk Gait when idle, to avoid undesired states and dont enter on walk mode when is idle and enter on reload mode or heal
			if (GetMovementAction() == EALSMovementAction::None && GetMovementState() == EALSMovementState::Grounded && GetVelocity() != FVector(0, 0, 0)) {
				ResetOriginalGait();
			}
		}
		else {
			SetOverlayState(DefaultState);
		}

		//Calls the BP implementation layer (only false value)
		//Its placed before setting the InReloadMode = false for avoid to show the ammo types HUD if its not in this mode when the pause key its pressed
		if (EquipSystemData.bEquipped == true && ReloadSystemData.bInReloadMode == true) {
			EnterReloadModeBP(bReloadMode);
		}

		ReloadSystemData.bInReloadMode = false;

		//Renables the player can equip again, to avoid to unequip while its interpolating the animation of the selected state
		GetWorld()->GetTimerManager().SetTimer(TimerHandleReloadMode, [&]() {
			EquipSystemData.bCanEquip = true;
			GetWorld()->GetTimerManager().ClearTimer(TimerHandleReloadMode);
		}, 0.3f, false);
	}
}

void AALSCharacter::Reloading_Implementation(bool bReloading, int32 TypeOfProjectile) {
	
	ReloadMode_Implementation(true);
	if (bReloading == true && CanPerformAction(EALSAction::Reload) == true) {

		//if there is assigned a candidate susccessfully
		TSubclassOf<AActor> SusccessfulReload = EquippableObjectReload_Interface->Execute_SetReadyLoadType(EquippedActorComponent->GetChildActor(), TypeOfProjectile);
		EquippableObjectReload_Interface->Execute_GetEquipableObjectData_Reload(EquippedActorComponent->GetChildActor(), EquippableObjectDataReload);

		if (SusccessfulReload != nullptr && (InventoryComponent == nullptr || InventoryComponent->InventoryComponentData.Inventory.Contains(SusccessfulReload) == true)) {
			//ReloadSystemData.bCanReload = true; it setted on the montage in a event
			ReloadSystemData.bCanReload = false;

			//For the shotgun: if there are not bullet on the chamber then reload it throught a montage to reload the chamber first
			if (EquippableObjectReload_Interface->Execute_CheckStatus(EquippedActorComponent->GetChildActor())) {
				GetMesh()->GetAnimInstance()->Montage_Play(EquippableObjectDataReload.ReloadSecondaryMontage, 1.0f);
			}
			else {
				GetMesh()->GetAnimInstance()->Montage_Play(EquippableObjectDataReload.ReloadMontage, 1.0f);
			}
		}
	}
	ReloadMode_Implementation(false);
}

void AALSCharacter::GetReloadSystemData_Implementation(FReloadSystemData& ReloadSystemStruct) {
	ReloadSystemStruct = ReloadSystemData;
}

void AALSCharacter::SetReloadSystemData_Implementation(FReloadSystemData ReloadSystemStruct) {
	ReloadSystemData = ReloadSystemStruct;
}

void AALSCharacter::EnterReloadModeBP_Implementation(bool bValue) {
}

void AALSCharacter::EnterReloadModeAction_Implementation(bool bValue) {
		ReloadMode_Implementation(bValue);
}
#pragma endregion

/*-= CRAFT KNOWLEDGE SYSTEM =-*/
#pragma region CraftKnowledgeSystem
void AALSCharacter::Learn_Implementation(FCrafteableObjectData Recipe) {
	CraftKnowledgeSystemData.CraftingList.Add(Recipe);
}

void AALSCharacter::Forget_Implementation(FCrafteableObjectData Recipe) {
	for (int32 i = 0; i < CraftKnowledgeSystemData.CraftingList.Num(); i++) {
		if (CraftKnowledgeSystemData.CraftingList[i].ObjectToCraft.ObjectClass == Recipe.ObjectToCraft.ObjectClass) {
			CraftKnowledgeSystemData.CraftingList.RemoveAt(i);
		}
	}
}

void AALSCharacter::GetCraftKnowledgeSystemData_Implementation(FCraftKnowledgeSystemData& CraftKnowledgeSystemStruct) {
	CraftKnowledgeSystemStruct = CraftKnowledgeSystemData;
}

void AALSCharacter::SetCraftKnowledgeSystemData_Implementation(FCraftKnowledgeSystemData CraftKnowledgeSystemStruct) {
	CraftKnowledgeSystemData = CraftKnowledgeSystemStruct;
}
#pragma endregion



/*-= DASH COMPONENT =-*/
#pragma region DashComponent
void AALSCharacter::DashAction_Implementation(bool bValue) {
	FVector LaunchDirection = GetLastMovementInputVector();
	LaunchDirection.Normalize();

	//Nothe: The additional check of LaunchDirection != FVector::ZeroVector its for only make the dash if there is a direction to go, this avoids specific cases like press and release the key to move and instantly press the dash
	
	if (bValue == true && DashComponent && DashComponent->DashComponentData.bCanDash == true && LaunchDirection != FVector::ZeroVector && CanPerformAction(EALSAction::Dash) == true) {
		
		//Set for the ALSBaseCharacter as cannnot change the movement action(no avoid if its crouching to not stand up)
		bCanChangeMovementState = false;
		//Cannot make other actions
		bBlockActions = true;
		
		//Set the vulnerability on
		if (ALSHealthComponent) {
			ALSHealthComponent->HealthComponentData.bIsInvulnerable = true;
		}
			
		//Gets the delay for renable the movement state depending if its crouched or not
		float DelayRenableMovementState;
		if (GetStance() != EALSStance::Crouching) {
			DelayRenableMovementState = DashComponent->DashComponentData.DelayReuseDash;
			DashComponent->StartDash_Implementation(false);
		}
		else {
			DelayRenableMovementState = DashComponent->DashComponentData.DelayReuseDashCrouch;
			DashComponent->StartDash_Implementation(true);
		}
	}
}
#pragma endregion

/*-= CONSUME COMPONENT =-*/
#pragma region ConsumeComponent
void AALSCharacter::ConsumeAction_Implementation(bool bValue) {
	if (bValue == true && ConsumeComponent && ConsumeComponent->ConsumeComponentData.bCanConsume == true && CanPerformAction(EALSAction::Consume) == true) {
		
		ConsumeComponent->StartConsuming_Implementation();

		//Important to call actorConsumable->Destroy(), its called in the ALSAnimNotifyConsume
	}
}
#pragma endregion

/*-= PICK COMPONENT =-*/
#pragma region PickComponent
void AALSCharacter::PickAction_Implementation(bool bValue) {
	if (bValue == true && PickComponent && PickComponent->PickComponentData.bCanPick == true && CanPerformAction(EALSAction::Pick) == true) {
		PickComponent->PickComponentData.bInPick = true;

		//If its sprinting set the state to running
		if (GetGait() == EALSGait::Sprinting) {
			SetDesiredGait(EALSGait::Running);
		}

		PickComponent->Pick_Implementation();
	}
}
#pragma endregion

/*-= INTERACT COMPONENT =-*/
#pragma region InteracComponent
void AALSCharacter::InteractAction_Implementation(bool bValue) {
	if (bValue == true && InteractComponent && InteractComponent->InteractComponentData.bCanInteract == true && CanPerformAction(EALSAction::Interact) == true) {
		InteractComponent->InteractComponentData.bInInteract = true;
		InteractComponent->Interact_Implementation();
		InteractComponent->InteractComponentData.bInInteract = false;
	}
}
#pragma endregion

/*-= OTHER METHODS =-*/
#pragma region OtherMethods
void AALSCharacter::ResetOriginalGait() {
	//This is for when sprints, enter on reload mode and leave that mode but the key to sprinting was pressed the enter again on sprint mode
	if (bWantToSprint == true) {
		SetDesiredGait(EALSGait::Sprinting);
		bWantToWalk = false;
	}
	else if (bWantToWalk == false) {
		SetDesiredGait(EALSGait::Running);
		bWantToWalk = false;
	}
	else {
		SetDesiredGait(EALSGait::Walking);
	}
}

void AALSCharacter::SetVisibleActorVisualsOnly(bool bNewVisibility) {
	GetMesh()->SetVisibility(bNewVisibility, true);
	ReloadShellAnchor->SetVisibility(bNewVisibility);
	Cast<IIEquipableObject_Basic>(EquippedActorComponent->GetChildActor())->Execute_SetVisibleMesh(EquippedActorComponent->GetChildActor(), bNewVisibility);
	EquippedPivot->SetVisibility(bNewVisibility, true);
	GetMesh()->bPauseAnims = !bNewVisibility;
	HeldObjectRoot->SetVisibility(bNewVisibility, true);
	ObjectAnchor->SetRelativeLocation(FVector(0,0,0));
}

void AALSCharacter::SetEquippableObjectVisible(bool bNewVisibility) {
	EquippedActorComponent->SetHiddenInGame(!bNewVisibility);
}

bool AALSCharacter::CanPerformAction(EALSAction Action) {
	switch (Action) {
	case EALSAction::Equip:
		return ReloadSystemData.bInReloadMode == false && UseSystemData.bInUse == false && CONSUME_COMPONENT_CONDITION && PICK_COMPONENT_CONDITION && GetMovementState() != EALSMovementState::Ragdoll && GetMovementAction() == EALSMovementAction::None;
	case EALSAction::Aim:
		return DASH_COMPONENT_CONDITION && GetMovementState() != EALSMovementState::Ragdoll && GetMovementAction() == EALSMovementAction::None;
	case EALSAction::Use:
		return  ReloadSystemData.bInReloadMode == false && EquippableObjectBasic_Interface && UseSystemData.bInUse == false && EquipSystemData.bInEquip == false && DASH_COMPONENT_CONDITION && CONSUME_COMPONENT_CONDITION && GetMovementState() != EALSMovementState::Ragdoll && GetMovementAction() == EALSMovementAction::None;
	case EALSAction::Dash:
		return  DASH_COMPONENT_CONDITION && CONSUME_COMPONENT_CONDITION && PICK_COMPONENT_CONDITION && GetMovementState() != EALSMovementState::Ragdoll && GetVelocity().Length() != 0.0 && GetMovementAction() == EALSMovementAction::None && GetMovementState() != EALSMovementState::Ragdoll && GetMovementState() != EALSMovementState::InAir;
	case EALSAction::Jump:
		return (ReloadSystemData.bInReloadMode == false && CONSUME_COMPONENT_CONDITION && PICK_COMPONENT_CONDITION && bBlockActions == false) || (GetMovementState() == EALSMovementState::Ragdoll);
	case EALSAction::Crouch:
		return false;
	case EALSAction::Consume:
		return GetMovementState() != EALSMovementState::Ragdoll && (ALSHealthComponent == nullptr || ALSHealthComponent->HealthComponentData.Life < ALSHealthComponent->HealthComponentData.MaxLife) && (InventoryComponent == nullptr || (ConsumeComponent && InventoryComponent->InventoryComponentData.Inventory.Contains(ConsumeComponent->ConsumeComponentData.ConsumableClass) == true)) && (ConsumeComponent->ConsumeComponentData.bInConsume == false) && ConsumeComponent->ConsumeComponentData.ConsumableClass && EquipSystemData.bInEquip == false && /*UseSystemData.bInUse == false && */PICK_COMPONENT_CONDITION && GetMovementAction() == EALSMovementAction::None && GetMovementState() == EALSMovementState::Grounded && DASH_COMPONENT_CONDITION && ReloadSystemData.bInReloadMode == false;
	case EALSAction::Pick:
		return GetMovementState() != EALSMovementState::Ragdoll && PickComponent && (!PickComponent->PickComponentData.PickableCandidates.IsEmpty()) && PickComponent->PickComponentData.bInPick == false && EquipSystemData.bEquipped == false && CONSUME_COMPONENT_CONDITION && DASH_COMPONENT_CONDITION && EquipSystemData.bInEquip == false && UseSystemData.bInUse == false && GetMovementAction() == EALSMovementAction::None && GetMovementState() == EALSMovementState::Grounded && DashComponent->DashComponentData.bInDash == false && ReloadSystemData.bInReloadMode == false;
	case EALSAction::Interact:
		return GetMovementState() != EALSMovementState::Ragdoll && DASH_COMPONENT_CONDITION && CONSUME_COMPONENT_CONDITION && PICK_COMPONENT_CONDITION && InteractComponent && (InteractComponent->InteractComponentData.ObjectToInteract) && UseSystemData.bInUse == false && GetMovementAction() == EALSMovementAction::None && GetMovementState() == EALSMovementState::Grounded && DashComponent->DashComponentData.bInDash == false && ReloadSystemData.bInReloadMode == false;
	case EALSAction::Reload:
		return ReloadSystemData.bCanReload == true && ReloadSystemData.bInReloadMode == true && GetMovementState() != EALSMovementState::Ragdoll;
	case EALSAction::ReloadMode:
		return GetMovementAction() == EALSMovementAction::None && GetMovementState() == EALSMovementState::Grounded && DASH_COMPONENT_CONDITION && CONSUME_COMPONENT_CONDITION && UseSystemData.bInUse == false && EquipSystemData.bInEquip == false;
	default:
		return false;
	}
}
#pragma endregion
