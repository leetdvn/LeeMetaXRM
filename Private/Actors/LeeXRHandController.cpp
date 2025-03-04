// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/LeeXRHandController.h"
#include "Definitions.h"
#include <EnhancedInputComponent.h>

ALeeXRHandController::ALeeXRHandController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	HandSkeletal = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandSkeletal"));
	HandSkeletal->SetupAttachment(MotionController);

	GrabSphere = CreateDefaultSubobject<USphereComponent>(TEXT("GrabSphereCollison"));
	GrabSphere->SetupAttachment(HandSkeletal);
	//Set Init Hand Left or Right

	ControllerType = ELeeXRHandType::LeeXRController;
}

void ALeeXRHandController::GraspObject()
{
	Super::GraspObject();
	LEE_SCOPE_CYCLE_COUNTER(ICTUController);

}

void ALeeXRHandController::GraspRelease()
{
	Super::GraspRelease();
	LEE_SCOPE_CYCLE_COUNTER(ICTUController);

}

void ALeeXRHandController::BeginPlay()
{
	Super::BeginPlay();
	
	SetHandSwitch(HandType == EControllerHand::Left);

	LEE_SCOPE_CYCLE_COUNTER(ICTUController);
	
	INC_MEMORY_STAT_BY(STAT_HandController, this->GetResourceSizeBytes(EResourceSizeMode::EstimatedTotal));

}

void ALeeXRHandController::SetInputComponent()
{
	Super::SetInputComponent();

	UInputComponent* PlayerInputComponent = GetWorld()->GetFirstPlayerController()->InputComponent;
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Started, this, &ALeeXRHandController::OnInputActionMove);
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ALeeXRHandController::OnInputActionMove);
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Completed, this, &ALeeXRHandController::OnInputActionMove);

	}
}

void ALeeXRHandController::OnInputActionMove(const FInputActionInstance& ActionInstance)
{
	
	ETriggerEvent TriggerEvent = ActionInstance.GetTriggerEvent();

	switch (TriggerEvent)
	{
	case ETriggerEvent::Triggered: {
		if (TeleportValid()) {
			bTeleportTraceActive = true;
			FVector StartPos = HandSkeletal->GetComponentToWorld().GetLocation();
			FVector ForwardVec = NiagaraComponent->GetForwardVector();
			TeleportTrace(StartPos, ForwardVec);
		}
		break;
	}
	case ETriggerEvent::Started:
			//StartTeleportTrace();
		break;
	case ETriggerEvent::Canceled:
		break;
	case ETriggerEvent::Completed:
		if (bTeleportTraceActive) {
			bTeleportTraceActive = false;
			TryTeleport();
		}
		break;
	default:
		break;
	}
}

void ALeeXRHandController::SetFingerAnimationPose(USkeletalMeshComponent* inComponet, const FInputActionInstance ActionInstance)
{

}

/// <summary>
/// Init Hand Setup
/// </summary>
void ALeeXRHandController::InittializeSetup()
{
	Super::InittializeSetup();

	GrabSphere->AttachToComponent(HandSkeletal, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "palm_r");
	GrabSphere->SetRelativeLocation(FVector(0.0f, 2.5f, -2.5f));

	WidgetInteraction->SetupAttachment(HandSkeletal,TEXT("Index3"));
}
