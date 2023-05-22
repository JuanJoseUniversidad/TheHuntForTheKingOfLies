// Copyright:       Copyright (C) 2022 Doğa Can Yanıkoğlu
// Source Code:     https://github.com/dyanikoglu/ALS-Community


#include "Character/ALSPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedPlayerInput.h"
#include "InputMappingContext.h"
#include "Engine/LocalPlayer.h"
#include "AI/ALSAIController.h"
#include "Character/ALSCharacter.h"
#include "Character/ALSPlayerCameraManager.h"
#include "Components/ALSDebugComponent.h"
#include "Kismet/GameplayStatics.h"

void AALSPlayerController::OnPossess(APawn* NewPawn)
{
	Super::OnPossess(NewPawn);
	PossessedCharacter = Cast<AALSBaseCharacter>(NewPawn);
	PossessedPlayerCharacter = Cast<AALSCharacter>(NewPawn);
	AimSystem = Cast<IIAimSystem>(PossessedPlayerCharacter);
	ReloadSystem = Cast<IIReloadSystem>(PossessedPlayerCharacter);

	if (!IsRunningDedicatedServer())
	{
		// Servers want to setup camera only in listen servers.
		SetupCamera();
	}

	SetupInputs();

	UALSDebugComponent* DebugComp = Cast<UALSDebugComponent>(PossessedCharacter->GetComponentByClass(UALSDebugComponent::StaticClass()));
	if (DebugComp)
	{
		DebugComp->OnPlayerControllerInitialized(this);
	}
}

void AALSPlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();
	PossessedCharacter = Cast<AALSBaseCharacter>(GetPawn());
	PossessedPlayerCharacter = Cast<AALSCharacter>(GetPawn());
	SetupCamera();
	SetupInputs();

	UALSDebugComponent* DebugComp = Cast<UALSDebugComponent>(PossessedCharacter->GetComponentByClass(UALSDebugComponent::StaticClass()));
	if (DebugComp)
	{
		DebugComp->OnPlayerControllerInitialized(this);
	}
}

void AALSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->ClearActionEventBindings();
		EnhancedInputComponent->ClearActionValueBindings();
		EnhancedInputComponent->ClearDebugKeyBindings();

		BindActions(DefaultInputMappingContext);
		BindActions(DebugInputMappingContext);
	}
	else
	{
		UE_LOG(LogTemp, Fatal, TEXT("ALS Community requires Enhanced Input System to be activated in project settings to function properly"));
	}
}

void AALSPlayerController::BindActions(UInputMappingContext* Context)
{
	if (Context)
	{
		const TArray<FEnhancedActionKeyMapping>& Mappings = Context->GetMappings();
		UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
		if (EnhancedInputComponent)
		{
			// There may be more than one keymapping assigned to one action. So, first filter duplicate action entries to prevent multiple delegate bindings
			TSet<const UInputAction*> UniqueActions;
			for (const FEnhancedActionKeyMapping& Keymapping : Mappings)
			{
				UniqueActions.Add(Keymapping.Action);
			}
			for (const UInputAction* UniqueAction : UniqueActions)
			{
				EnhancedInputComponent->BindAction(UniqueAction, ETriggerEvent::Triggered, Cast<UObject>(this), UniqueAction->GetFName());
			}
		}
	}
}

void AALSPlayerController::SetPauseBP_Implementation(bool bValue) {
}

void AALSPlayerController::SetupInputs()
{
	if (PossessedCharacter)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			FModifyContextOptions Options;
			Options.bForceImmediately = 1;
			Subsystem->AddMappingContext(DefaultInputMappingContext, 1, Options);
			UALSDebugComponent* DebugComp = Cast<UALSDebugComponent>(PossessedCharacter->GetComponentByClass(UALSDebugComponent::StaticClass()));
			if (DebugComp)
			{
				// Do only if we have debug component
				Subsystem->AddMappingContext(DebugInputMappingContext, 0, Options);
			}
		}
	}
}

void AALSPlayerController::SetupCamera()
{
	// Call "OnPossess" in Player Camera Manager when possessing a pawn
	AALSPlayerCameraManager* CastedMgr = Cast<AALSPlayerCameraManager>(PlayerCameraManager);
	if (PossessedCharacter && CastedMgr)
	{
		CastedMgr->OnPossess(PossessedCharacter);
	}
}

void AALSPlayerController::ForwardMovementAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && PossessedPlayerCharacter && PossessedPlayerCharacter->bBlockActions == false)
	{
		PossessedCharacter->ForwardMovementAction(Value.GetMagnitude());
	}
}

void AALSPlayerController::RightMovementAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && PossessedPlayerCharacter && PossessedPlayerCharacter->bBlockActions == false)
	{
		PossessedCharacter->RightMovementAction(Value.GetMagnitude());
	}
}

void AALSPlayerController::CameraUpAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		PossessedCharacter->CameraUpAction(Value.GetMagnitude());
	}
}

void AALSPlayerController::CameraRightAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		PossessedCharacter->CameraRightAction(Value.GetMagnitude());
	}
}

void AALSPlayerController::JumpAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && PossessedPlayerCharacter && PossessedPlayerCharacter->CanPerformAction(EALSAction::Jump))
	{
		PossessedCharacter->JumpAction(Value.Get<bool>());
	}
}

//Equip action
void AALSPlayerController::EquipAction(const FInputActionValue& Value){
	if (PossessedPlayerCharacter){
		PossessedPlayerCharacter->EquipAction(Value.Get<bool>());
	}
}

//Use action
void AALSPlayerController::UseAction(const FInputActionValue& Value){
	if (PossessedPlayerCharacter) {
		PossessedPlayerCharacter->UseAction(Value.Get<bool>());
	}
}

//Enter reload mode action [Depecrated due a desing decision (entering on a reload mode by holding a key in a action game in a action game its a bad idea, instead reloads the bullet directly)]
void AALSPlayerController::ReloadModeAction(const FInputActionValue& Value){
	if (PossessedPlayerCharacter) {
		
		//PossessedPlayerCharacter->EnterReloadModeAction(Value.Get<bool>());
	}
}

//Reload first type ammo action
void AALSPlayerController::ReloadFirstAmmoAction(const FInputActionValue& Value) {
	if (PossessedPlayerCharacter && PossessedPlayerCharacter->bBlockActions == false) {
		//PossessedPlayerCharacter->bWantToReload = Value.Get<bool>();
		ReloadSystem->Execute_Reloading(PossessedPlayerCharacter, Value.Get<bool>(), 0);
	}
}

//Reload second type ammo action
void AALSPlayerController::ReloadSecondAmmoAction(const FInputActionValue& Value) {
	if (PossessedPlayerCharacter && PossessedPlayerCharacter->bBlockActions == false) {
		//PossessedPlayerCharacter->bWantToReload = Value.Get<bool>();
		ReloadSystem->Execute_Reloading(PossessedPlayerCharacter, Value.Get<bool>(), 1);
	}
}

//Reload third type ammo action
void AALSPlayerController::ReloadThirdAmmoAction(const FInputActionValue& Value) {
	if (PossessedPlayerCharacter && PossessedPlayerCharacter->bBlockActions == false) {
		//PossessedPlayerCharacter->bWantToReload = Value.Get<bool>();
		ReloadSystem->Execute_Reloading(PossessedPlayerCharacter, Value.Get<bool>(), 2);
	}
}

//Dash action
void AALSPlayerController::DashAction(const FInputActionValue& Value){
	if (PossessedPlayerCharacter) {
		PossessedPlayerCharacter->DashAction(Value.Get<bool>());
	}
}

//Consume action
void AALSPlayerController::ConsumeAction(const FInputActionValue& Value){
	if (PossessedPlayerCharacter) {
		PossessedPlayerCharacter->ConsumeAction(Value.Get<bool>());
	}
}

//Pick action
void AALSPlayerController::PickAction(const FInputActionValue& Value){
	if (PossessedPlayerCharacter) {
		PossessedPlayerCharacter->PickAction(Value.Get<bool>());
	}
}

//Interact action
void AALSPlayerController::InteractAction(const FInputActionValue& Value) {
	//If isnt in pause mode
	if (PossessedPlayerCharacter && IsMoveInputIgnored() == false) {
		PossessedPlayerCharacter->InteractAction(Value.Get<bool>());
	}
}

//Pause action
void AALSPlayerController::PauseAction(const FInputActionValue& Value) {
	if (PossessedPlayerCharacter) {
		//Restart every key action
		SimulateAllActionsKeys();
		
		//Disable movement character
		SetIgnoreMoveInput(true);

		//Set aim to false
		AimSystem->Execute_Aim(PossessedPlayerCharacter, false);

		//Set pause
		SetPauseBP(Value.Get<bool>());
	}
}

// EDITED
void AALSPlayerController::SprintAction(const FInputActionValue& Value)
{


	if (PossessedCharacter && PossessedPlayerCharacter)
	{
		//If its on reload mode and pulse the sprint button then enters on running mode
		if (Value.Get<bool>() == true && PossessedPlayerCharacter->bWantToWalk == true && PossessedPlayerCharacter->ReloadSystemData.bInReloadMode == true) {
			PossessedPlayerCharacter->bWantToWalk = false;
			PossessedPlayerCharacter->SetDesiredGait(EALSGait::Running);
		}

		PossessedPlayerCharacter->bWantToSprint = Value.Get<bool>();
		
		if (PossessedPlayerCharacter->ReloadSystemData.bInReloadMode == false && (PossessedPlayerCharacter->ConsumeComponent == nullptr || PossessedPlayerCharacter->ConsumeComponent->ConsumeComponentData.bInConsume == false) && (PossessedPlayerCharacter->PickComponent == nullptr || PossessedPlayerCharacter->PickComponent->PickComponentData.bInPick == false)) {
			PossessedCharacter->SprintAction(Value.Get<bool>());
		}
	}
}

void AALSPlayerController::AimAction(const FInputActionValue& Value)
{
	//--
	if (PossessedPlayerCharacter && PossessedPlayerCharacter->AimSystemData.bCanAim == true && IsMoveInputIgnored() == false) {
		//Aim action
		if (PossessedCharacter)
		{
			PossessedCharacter->AimAction(Value.Get<bool>());
		}
		
		AimSystem->Execute_Aim(PossessedPlayerCharacter, Value.Get<bool>());
	}
}

void AALSPlayerController::CameraTapAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		PossessedCharacter->CameraTapAction();
	}
}

void AALSPlayerController::CameraHeldAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		PossessedCharacter->CameraHeldAction();
	}
}

void AALSPlayerController::StanceAction(const FInputActionValue& Value)
{
	//--
	if (PossessedCharacter && Value.Get<bool>() && PossessedPlayerCharacter->ReloadSystemData.bInReloadMode == false && PossessedPlayerCharacter->bBlockActions == false)
	{
		PossessedCharacter->StanceAction();
	}
}

// EDITED
void AALSPlayerController::WalkAction(const FInputActionValue& Value)
{
	

	if (PossessedCharacter && Value.Get<bool>() && (PossessedPlayerCharacter->ConsumeComponent == nullptr || PossessedPlayerCharacter->ConsumeComponent->ConsumeComponentData.bInConsume == false))
	{
		if (PossessedPlayerCharacter->bWantToWalk == true) {
			PossessedPlayerCharacter->bWantToWalk = false;
		}else{
			PossessedPlayerCharacter->bWantToWalk = true;
		}

		PossessedCharacter->WalkAction();
	}
}

void AALSPlayerController::RagdollAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		PossessedCharacter->RagdollAction();
	}
}

void AALSPlayerController::VelocityDirectionAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		PossessedCharacter->VelocityDirectionAction();
	}
}

void AALSPlayerController::LookingDirectionAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		PossessedCharacter->LookingDirectionAction();
	}
}

void AALSPlayerController::DebugToggleHudAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		UALSDebugComponent* DebugComp = Cast<UALSDebugComponent>(PossessedCharacter->GetComponentByClass(UALSDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->ToggleHud();
		}
	}
}

void AALSPlayerController::DebugToggleDebugViewAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		UALSDebugComponent* DebugComp = Cast<UALSDebugComponent>(PossessedCharacter->GetComponentByClass(UALSDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->ToggleDebugView();
		}
	}
}

void AALSPlayerController::DebugToggleTracesAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		UALSDebugComponent* DebugComp = Cast<UALSDebugComponent>(PossessedCharacter->GetComponentByClass(UALSDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->ToggleTraces();
		}
	}
}

void AALSPlayerController::DebugToggleShapesAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		UALSDebugComponent* DebugComp = Cast<UALSDebugComponent>(PossessedCharacter->GetComponentByClass(UALSDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->ToggleDebugShapes();
		}
	}
}

void AALSPlayerController::DebugToggleLayerColorsAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		UALSDebugComponent* DebugComp = Cast<UALSDebugComponent>(PossessedCharacter->GetComponentByClass(UALSDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->ToggleLayerColors();
		}
	}
}

void AALSPlayerController::DebugToggleCharacterInfoAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		UALSDebugComponent* DebugComp = Cast<UALSDebugComponent>(PossessedCharacter->GetComponentByClass(UALSDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->ToggleCharacterInfo();
		}
	}
}

void AALSPlayerController::DebugToggleSlomoAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		UALSDebugComponent* DebugComp = Cast<UALSDebugComponent>(PossessedCharacter->GetComponentByClass(UALSDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->ToggleSlomo();
		}
	}
}

void AALSPlayerController::DebugFocusedCharacterCycleAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		UALSDebugComponent* DebugComp = Cast<UALSDebugComponent>(PossessedCharacter->GetComponentByClass(UALSDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->FocusedDebugCharacterCycle(Value.GetMagnitude() > 0);
		}
	}
}

void AALSPlayerController::DebugToggleMeshAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		UALSDebugComponent* DebugComp = Cast<UALSDebugComponent>(PossessedCharacter->GetComponentByClass(UALSDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->ToggleDebugMesh();
		}
	}
}

void AALSPlayerController::DebugOpenOverlayMenuAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		UALSDebugComponent* DebugComp = Cast<UALSDebugComponent>(PossessedCharacter->GetComponentByClass(UALSDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->OpenOverlayMenu(Value.Get<bool>());
		}
	}
}

void AALSPlayerController::DebugOverlayMenuCycleAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		UALSDebugComponent* DebugComp = Cast<UALSDebugComponent>(PossessedCharacter->GetComponentByClass(UALSDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->OverlayMenuCycle(Value.GetMagnitude() > 0);
		}
	}
}

void AALSPlayerController::SimulateAllActionsKeys(float Value) {
	//Gets all the actions from the InputMappingContext and simulates each action key
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	FInputActionValue ActionValue(Value);

	for (const FEnhancedActionKeyMapping& Keymapping : DefaultInputMappingContext->GetMappings()) {
		Subsystem->GetPlayerInput()->InjectInputForAction(Keymapping.Action, ActionValue);
	}
}
