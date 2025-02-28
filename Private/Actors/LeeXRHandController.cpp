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
	GrabSphere->AttachToComponent(HandSkeletal, FAttachmentTransformRules::SnapToTargetNotIncludingScale,"palm_r");
	GrabSphere->SetRelativeLocation(FVector(0.0f, 2.5f, -2.5f));

	//Set Init Hand Left or Right

	ControllerType = ELeeXRHandType::LeeXRController;
}

void ALeeXRHandController::GraspObject()
{
	Super::GraspObject();
	LEE_SCOPE_CYCLE_COUNTER(ICTUController);
	LeeScreenLog("Grasp Object :%s", FColor::Green, *GetName());

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
	SetFingerAnimationPose(HandSkeletal, ActionInstance);
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
	Super::InittializeSetup();
}
