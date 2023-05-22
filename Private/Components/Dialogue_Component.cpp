// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Dialogue_Component.h"

// Sets default values for this component's properties
UDialogue_Component::UDialogue_Component()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	CannotFocusTag = "Status.CannotFocus";

	//SetComponentTickEnabled(false);
}

#pragma region IInteractable_Basic
bool UDialogue_Component::Interact_Implementation(AActor* Interactor) {

	
	//Focus owner to the interactor(for example. focus the character who triggered the dialogue)
	CharacterOwner = Cast<ACharacter>(GetOwner());
	if (UAIBlueprintHelperLibrary::GetAIController(GetOwner())) {
		if (!GetOwner()->ActorHasTag(CannotFocusTag)) {
			UAIBlueprintHelperLibrary::GetAIController(GetOwner())->SetFocus(Interactor);
		}
		OwnerMovementMode = CharacterOwner->GetCharacterMovement()->GetGroundMovementMode();
		CharacterOwner->GetCharacterMovement()->DisableMovement();
	}

	//Focus interactor to owner (check if its the player or if not has an AIController) with interpolation on the camera trayectory if it's a player the interactor. If it's an AI the just use the setFocus 
	CharacterInteractor = Cast<ACharacter>(Interactor);
	if (CharacterInteractor) {
		if (!UAIBlueprintHelperLibrary::GetAIController(CharacterInteractor)) {
			CharacterInteractor->GetCharacterMovement()->StopMovementImmediately();
			InterpolatedRotation = CharacterInteractor->GetController()->GetControlRotation();
			bInterpolationCameraComplete = false;
			SetComponentTickEnabled(true);
		}
		else {
			UAIBlueprintHelperLibrary::GetAIController(CharacterInteractor)->SetFocus(GetOwner());

			InteractorMovementMode = CharacterInteractor->GetCharacterMovement()->GetGroundMovementMode();
			CharacterInteractor->GetCharacterMovement()->DisableMovement();
		}
	}

	SetInteractor_Implementation(Interactor);

	//Creates the interface
	CreateDialogInterface();

	//Plays the audio
	if (Interactable_Basic.InteractSoundCue) {
		ACharacter* Character = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetCharacter();
		if (Interactable_Basic.bAttachedSound == true) {
			UGameplayStatics::SpawnSoundAttached(Interactable_Basic.InteractSoundCue, GetOwner()->GetRootComponent());
		}
		else {
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), Interactable_Basic.InteractSoundCue, GetOwner()->GetActorLocation());
		}
	}

	return true;
}

bool UDialogue_Component::EndInteract_Implementation() {
	SetComponentTickEnabled(false);
	bInterpolationCameraComplete = false;

	//Clear the focus of the owner of the component and reset the original movement mode
	UAIBlueprintHelperLibrary::GetAIController(GetOwner())->ClearFocus(EAIFocusPriority::Gameplay);
	CharacterOwner->GetCharacterMovement()->SetMovementMode(OwnerMovementMode);
	
	//Clear the focus if it's and AI the interactor and reset the original movement mode
	if (UAIBlueprintHelperLibrary::GetAIController(CharacterInteractor)) {
		CharacterInteractor->GetCharacterMovement()->SetMovementMode(InteractorMovementMode);
		UAIBlueprintHelperLibrary::GetAIController(CharacterInteractor)->ClearFocus(EAIFocusPriority::Gameplay);
	}

	return true;
}

void UDialogue_Component::GetInteractableData_Basic_Implementation(FInteractableData_Basic& InteractableStruct) {
	InteractableStruct = Interactable_Basic;
}

void UDialogue_Component::SetInteractableData_Basic_Implementation(FInteractableData_Basic InteractableStruct) {
	Interactable_Basic = InteractableStruct;
}
#pragma endregion



#pragma region IDialogueComponent
void UDialogue_Component::ConstructFullName_Implementation(FName Name, FName Nickname) {
	if (Name != "" && Name != "None") {
		DialogueComponentData.Name = Name;
	}

	if (Nickname != "" && Nickname != "None") {
		DialogueComponentData.Nickname = Nickname;
	}
}

FName UDialogue_Component::GetFullName_Implementation() {
	FString FullName = DialogueComponentData.Name.ToString();

	if (DialogueComponentData.Nickname != "" && DialogueComponentData.Nickname != "None") {
		FullName.Append(". " + DialogueComponentData.Nickname.ToString());
	}

	return FName(*FullName);
}

void UDialogue_Component::SetInteractor_Implementation(AActor* Interactor) {
	DialogueComponentData.InteractorReference = Interactor;
}

AActor* UDialogue_Component::GetInteractor_Implementation() {
	return DialogueComponentData.InteractorReference;
}

void UDialogue_Component::GetDialogueComponentData_Implementation(FDialogueComponentData& DialogueComponentStruct) {
	DialogueComponentStruct = DialogueComponentData;
}

void UDialogue_Component::SetDialogueComponentData_Implementation(FDialogueComponentData DialogueComponentStruct) {
	DialogueComponentData = DialogueComponentStruct;
}
#pragma endregion



#pragma region OtherMethods
void UDialogue_Component::CreateDialogInterface_Implementation() {
}
#pragma endregion

// Called when the game starts
void UDialogue_Component::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDialogue_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Interpolates the camera to the head of the owner of the component 
	if (CharacterOwner && CharacterInteractor && bInterpolationCameraComplete == false) {
		
		//If the interactor's head its more or less in the same heigh as the owner's of the dialog component then adds some offset to get a better view of the dialog
		FVector DistanceDiferenceHeads = CharacterInteractor->GetMesh()->GetSocketLocation("FP_Camera") - CharacterOwner->GetMesh()->GetSocketLocation("FP_Camera");
		if (DistanceDiferenceHeads.Z > -DialogueThresholfForOffset && DistanceDiferenceHeads.Z < DialogueThresholfForOffset) {
			OffsetCameraRotator = DialogueOffset;
		}
		else {
			OffsetCameraRotator = 0.0f;
		}

		InterpolatedRotation = FMath::RInterpTo(InterpolatedRotation, (UKismetMathLibrary::FindLookAtRotation(CharacterInteractor->GetMesh()->GetSocketLocation("FP_Camera"), CharacterOwner->GetMesh()->GetSocketLocation("FP_Camera")).Add(OffsetCameraRotator,0,0)), DeltaTime, 1.f);
		CharacterInteractor->GetController()->SetControlRotation(InterpolatedRotation);
	}


	if (CharacterOwner && CharacterInteractor && FMath::TruncToInt(InterpolatedRotation.Yaw) == FMath::TruncToInt(UKismetMathLibrary::FindLookAtRotation(CharacterInteractor->GetMesh()->GetSocketLocation("FP_Camera"), CharacterOwner->GetMesh()->GetSocketLocation("FP_Camera")).Add(OffsetCameraRotator, 0, 0).Yaw)){
		bInterpolationCameraComplete = true;
	}

	//Check distance between actors if are out of range then close the dialog by calling its function of the interface
	if (CharacterOwner && CharacterInteractor && FVector::Distance(CharacterOwner->GetActorLocation(), CharacterInteractor->GetActorLocation()) > DialogDistance) {
		SetComponentTickEnabled(false);
		FOutputDeviceNull ar;
		InterfaceReference->CallFunctionByNameWithArguments(*DialogCloseFunctionName, ar, NULL, true);
	}
}

