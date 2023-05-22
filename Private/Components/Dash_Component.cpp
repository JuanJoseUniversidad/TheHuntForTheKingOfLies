// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Dash_Component.h"
#include "NiagaraComponent.h"
#include "Animation/AnimMontage.h"
#include "Components/SkeletalMeshComponent.h"

//TODO Que ejecute la animacion conforme al dash y su direccion, si esta la opcion activada
//TODO Obtener la direccion y con Getrotation->GetRotationXVector y con esto dependiendo del enumerador dado modificar sus componentes positivas y negativas


// Sets default values for this component's properties
UDash_Component::UDash_Component()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	//DashEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara efect component"));
	
	DashEffectScaleOffset = FVector(1.0f, 1.0f, 1.0f);

	bMakeVisibleCharacter = false;
}


// Called when the game starts
void UDash_Component::BeginPlay()
{
	Super::BeginPlay();

	// ...
	Owner = Cast<ACharacter>(GetOwner());
	//DashEffectComponent->AttachToComponent(Owner->GetMesh(),FAttachmentTransformRules::KeepRelativeTransform);
	//DashEffectComponent->bAutoActivate = false;
	//DashEffectComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 92.0f));

	//Gets the first visible skeletal mesh to execute the montages
	TArray<UActorComponent*> SkeletalMeshComponents = Owner->GetComponentsByClass(USkeletalMeshComponent::StaticClass());
	for (int32 i = 0; i < SkeletalMeshComponents.Num(); i++) {
		USkeletalMeshComponent* SkeletalMeshComponent = Cast<USkeletalMeshComponent>(SkeletalMeshComponents[i]);
		if (SkeletalMeshComponent->IsVisible()) {
			SkeletalMeshToAffect = SkeletalMeshComponent;
			break;
		}
	}
}


// Called every frame
void UDash_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}



void UDash_Component::StartDash_Implementation(bool IsCrouch, EALSEightDirection AlternativeDirection) {
	//Execute all lambdas expressions
	for (TFunction<void()> functionLambda : DashComponentData.StartDashFunctions) {
		functionLambda();
	}
	Owner->GetCapsuleComponent()->BodyInstance.bLockRotation = true;
	Owner->GetMesh()->BodyInstance.bLockRotation = true;

	VelocityZPreDash = Owner->GetCharacterMovement()->Velocity.Z;
	Owner->GetCharacterMovement()->Velocity.X = 0;
	Owner->GetCharacterMovement()->Velocity.Y = 0;

	DashComponentData.bCanDash = false;
	DashComponentData.bInDash = true;

	//Play sound
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DashSoundCue, Owner->GetActorLocation());

	//Execute dash effect
	//DashEffectComponent->SetVisibility(true);
	//DashEffectComponent->Activate(true);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DashEffect, Owner->GetActorLocation() + DashEffectLocationOffset, FRotator::ZeroRotator, DashEffectScaleOffset);

	//Execute the feedback force
	if (DashFeedbackForce) {
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->ClientPlayForceFeedback(DashFeedbackForce);
	}

	//Play camera shake
	APlayerController* PController = Cast<APlayerController>(Owner->GetController());
	if (PController) {
		PController->ClientStartCameraShake(DashCameraShake, ScaleDashCamShake);
	}


	FVector LaunchDirection;
	if (AlternativeDirection != EALSEightDirection::None) {
		//Gets the forward direction
		LaunchDirection = Owner->GetActorRotation().Vector();
		FVector DirectionVectorCopy = LaunchDirection;

		//Calculate the direction to go
		switch (AlternativeDirection) {
		case EALSEightDirection::Right:
			LaunchDirection = FVector(LaunchDirection.Y * -1, LaunchDirection.X, 0);
			break;
		case EALSEightDirection::Left:
			LaunchDirection = FVector(LaunchDirection.Y, LaunchDirection.X * -1, 0);
			break;
		case EALSEightDirection::Backward:
			LaunchDirection *= -1;
			break;
		case EALSEightDirection::ForwardRight:
			LaunchDirection = LaunchDirection + FVector(LaunchDirection.Y * -1, LaunchDirection.X, 0);
			break;
		case EALSEightDirection::ForwardLeft:
			LaunchDirection = LaunchDirection + FVector(LaunchDirection.Y, LaunchDirection.X * -1, 0);
			break;
		case EALSEightDirection::BackwardRight:
			LaunchDirection = (LaunchDirection * -1) + FVector(LaunchDirection.Y * -1, LaunchDirection.X, 0);
			break;
		case EALSEightDirection::BackwardLeft:
			LaunchDirection = (LaunchDirection * -1) + FVector(LaunchDirection.Y, LaunchDirection.X * -1, 0);
			break;
		case EALSEightDirection::None:
			LaunchDirection = FVector(0.0f, 0.0f, 0.0f);
			break;
		default:
			break;
		}
	}
	else {
		LaunchDirection = Owner->GetLastMovementInputVector();
	}
	
	LaunchDirection.Normalize();


	//Execute animation
	if (DirectionAnimations.Contains(AlternativeDirection)) {
		SkeletalMeshToAffect->GetAnimInstance()->Montage_Play(DirectionAnimations[AlternativeDirection]);
	}

	if (IsCrouch == false) {
		//Owner->ACharacter::LaunchCharacter(LaunchDirection * DashComponentData.DashPower, true, true);
		Owner->GetCharacterMovement()->AddImpulse(LaunchDirection * DashComponentData.DashPower, true);
		//ACharacter::LaunchCharacter(FVector(GetLastMovementInputVector().X * dashPower, GetLastMovementInputVector().Y * dashPower, 0), true, true);
		//ACharacter::LaunchCharacter(GetActorForwardVector() * dashPower, true, true);
		EndDash_Implementation(DashComponentData.DelayReuseDash, DashComponentData.DelayEnableAnimations, DashComponentData.DelayVisiblePlayer);
	}
	else {
		//Owner->ACharacter::LaunchCharacter(LaunchDirection * DashComponentData.DashPowerCrouch, true, true);
		Owner->GetCharacterMovement()->AddImpulse(LaunchDirection * DashComponentData.DashPower, true);
		EndDash_Implementation(DashComponentData.DelayReuseDashCrouch, DashComponentData.DelayEnableAnimationsCrouch, DashComponentData.DelayVisiblePlayerCrouch);
	}
}

void UDash_Component::EndDash_Implementation(float ReuseDashDelay, float EnableAnimationsDelay, float VisiblePlayerDelay) {
	GetWorld()->GetTimerManager().SetTimer(TimerHandleDash, [&]() {
		DashComponentData.bCanDash = true;

		//Play sound
		if (DashAvalibleSoundCue) {
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), DashAvalibleSoundCue, Owner->GetActorLocation());
		}

		Owner->GetCapsuleComponent()->BodyInstance.bLockRotation = false;
		Owner->GetMesh()->BodyInstance.bLockRotation = false;

		//Play effect
		if (DashAvalibleEffect) {
			if (bEffectDashAttached == true) {
				UNiagaraFunctionLibrary::SpawnSystemAttached(DashAvalibleEffect, Owner->GetMesh(), "none", FVector(0, 0, 0), FRotator(0, 0, 0), EAttachLocation::SnapToTarget, true);
			}
			else {
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DashAvalibleEffect, Owner->GetActorLocation());
			}
		}

		GetWorld()->GetTimerManager().ClearTimer(TimerHandleDash);
	}, ReuseDashDelay, false);

	GetWorld()->GetTimerManager().SetTimer(TimerHandleDashAnim, [&]() {
		//GetCharacterMovement()->SetPlaneConstraintEnabled(false);
		
		//Execute the second lambda expression
		DashComponentData.EndDashFunctions[0]();

		DashComponentData.bInDash = false;


		GetWorld()->GetTimerManager().ClearTimer(TimerHandleDashAnim);
		}, EnableAnimationsDelay, false);

	GetWorld()->GetTimerManager().SetTimer(TimerHandleDashEffect, [&]() {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DashEffect, Owner->GetActorLocation() + DashEffectLocationOffset, FRotator::ZeroRotator, DashEffectScaleOffset);

	}, VisiblePlayerDelay - 0.075f, false);


	GetWorld()->GetTimerManager().SetTimer(TimerHandleDashVisible, [&]() {
		
		//Execute the third lambda expression
		DashComponentData.EndDashFunctions[1]();

		//Stop dash effect
		//DashEffectComponent->SetVisibility(false);
		//DashEffectComponent->Deactivate();


		GetWorld()->GetTimerManager().ClearTimer(TimerHandleDashVisible);
		}, VisiblePlayerDelay, false);
}

void UDash_Component::GetDashComponentData_Implementation(FDashComponentData& DashComponentStruct) {
	DashComponentStruct = DashComponentData;
}

void UDash_Component::SetDashComponentData_Implementation(FDashComponentData DashComponentStruct) {
	DashComponentData = DashComponentStruct;
}

bool UDash_Component::GetMakeVisibleCharacter() {
	return bMakeVisibleCharacter;
}