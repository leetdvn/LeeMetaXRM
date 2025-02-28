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
	GrabSphere->SetupAttachment(MotionController);

	//Set Init Hand Left or Right
	SetHandSwitch(false);

	ControllerType = ELeeXRHandType::LeeXRController;
}

void ALeeXRHandController::SetHandSwitch(bool isLeft)
{
	LEE_SCOPE_CYCLE_COUNTER(ICTUController);

	FString HandName = isLeft ? "Left" : "Right";

	MotionController->MotionSource = FName(*HandName);

	FVector LeftLoc = FVector(3.0f, -3.5f, 4.5f);
	FVector RightLoc = FVector(-8.0f, 3.5f, 0.0f);
	//===================================
	FRotator LeftRot = FRotator(90.f, -25.0f, -180.0f);
	FRotator RightRot = FRotator(7.8f, 80.0f, 0.0f);

	if (HandSkeletal)
	{
		HandSkeletal->SetRelativeLocation(isLeft ? LeftLoc : RightLoc);
		HandSkeletal->SetRelativeRotation(isLeft ? LeftRot : RightRot);
	}
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
	
	InittializeSetup();

	LEE_SCOPE_CYCLE_COUNTER(ICTUController);
	
	INC_MEMORY_STAT_BY(STAT_HandController, this->GetResourceSizeBytes(EResourceSizeMode::EstimatedTotal));

}

void ALeeXRHandController::SetFingerAnimationPose(USkeletalMeshComponent* inComponet, const FInputActionInstance ActionInstance)
{
	if (inComponet == nullptr) {
		LeeScreenLog("Hand Skeletal is Null", FColor::Red);
		return;
	};
	LEE_SCOPE_CYCLE_COUNTER(ICTUController);

	float ActValue = ActionInstance.GetValue().Get<float>();
	
	ETriggerEvent &&inEventType = ActionInstance.GetTriggerEvent();

	float PoseValueStartCancel = inEventType == ETriggerEvent::Completed ? ActValue : 0.0f;

	float PoseValueCancelCompleted = inEventType == ETriggerEvent::Triggered ? ActValue : 0.0f;

	FString EventName = UEnum::GetValueAsString(inEventType);

	FString ActName = ActionInstance.GetSourceAction()->GetName();

	ULeeXRAnimInstance* AnimIns = CastChecked<ULeeXRAnimInstance>(inComponet->GetAnimInstance());
	if (!AnimIns) return;

	if (ActName == this->IA_FingerPoint->GetName())
	{
		AnimIns->PoseAlphaPoint = PoseValueStartCancel;
	}
	else if (ActName == this->IA_CurlIndex->GetName())
	{
		AnimIns->PoseAlphaIndexCurl = PoseValueCancelCompleted;
	}
	else if (ActName == this->IA_HandThumpUp->GetName())
	{
		AnimIns->PoseAlphaThumbUp = PoseValueStartCancel;
	}
	else if (ActName == this->IA_Grasp->GetName())
	{
		AnimIns->PoseAlphaGrasp = PoseValueCancelCompleted;

		PoseValueCancelCompleted == 1 ?
			GraspObject() : GraspRelease();
	}

}

void ALeeXRHandController::OnFingerAnimation(const FInputActionInstance& ActionInstance)
{
	///Set Animation Pose
	return SetFingerAnimationPose(HandSkeletal, ActionInstance);
}


void ALeeXRHandController::SetInputComponent()
{
	Super::SetInputComponent();

	UInputComponent* PlayerInputComponent = GetWorld()->GetFirstPlayerController()->InputComponent;
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Grasp, ETriggerEvent::Triggered, this, &ALeeXRHandController::OnFingerAnimation);
		EnhancedInputComponent->BindAction(IA_Grasp, ETriggerEvent::Completed, this, &ALeeXRHandController::OnFingerAnimation);
		EnhancedInputComponent->BindAction(IA_Grasp, ETriggerEvent::Canceled, this, &ALeeXRHandController::OnFingerAnimation);
		//EnhancedInputComponent->BindAction(IA_Grasp, ETriggerEvent::Triggered, this, &ALeeXRHandBase::OnHandTrigger);

		EnhancedInputComponent->BindAction(IA_HandThumpUp, ETriggerEvent::Started, this, &ALeeXRHandController::OnFingerAnimation);
		EnhancedInputComponent->BindAction(IA_HandThumpUp, ETriggerEvent::Canceled, this, &ALeeXRHandController::OnFingerAnimation);
		EnhancedInputComponent->BindAction(IA_HandThumpUp, ETriggerEvent::Completed, this, &ALeeXRHandController::OnFingerAnimation);

		EnhancedInputComponent->BindAction(IA_CurlIndex, ETriggerEvent::Triggered, this, &ALeeXRHandController::OnFingerAnimation);
		EnhancedInputComponent->BindAction(IA_CurlIndex, ETriggerEvent::Canceled, this, &ALeeXRHandController::OnFingerAnimation);
		EnhancedInputComponent->BindAction(IA_CurlIndex, ETriggerEvent::Completed, this, &ALeeXRHandController::OnFingerAnimation);

		EnhancedInputComponent->BindAction(IA_FingerPoint, ETriggerEvent::Started, this, &ALeeXRHandController::OnFingerAnimation);
		EnhancedInputComponent->BindAction(IA_FingerPoint, ETriggerEvent::Canceled, this, &ALeeXRHandController::OnFingerAnimation);
		EnhancedInputComponent->BindAction(IA_FingerPoint, ETriggerEvent::Completed, this, &ALeeXRHandController::OnFingerAnimation);
	}
	LEE_SCOPE_CYCLE_COUNTER(ICTUController);
}

/// <summary>
/// Init Hand Setup
/// </summary>
void ALeeXRHandController::InittializeSetup()
{
	LEE_SCOPE_CYCLE_COUNTER(ICTUController);

	///Load the hand assets
	GrabSphere->SetSphereRadius(12.0f);
#if WITH_EDITOR
	if (WidgetInteraction) {
		WidgetInteraction->bShowDebug = true;
		WidgetInteraction->DebugColor = FColor::Green;
		WidgetInteraction->TraceChannel = ECollisionChannel::ECC_WorldDynamic;
	}
#endif
}
