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
}

void ALeeXRHandController::PlayAnimAction(const EFingerInputType& inFinger,float inActionValue, bool isTrigger)
{

	LEE_SCOPE_CYCLE_COUNTER(ICTUController);

	if (!HandSkeletal) return;

	ULeeXRAnimInstance* AnimIns = Cast<ULeeXRAnimInstance>(HandSkeletal->GetAnimInstance());
	if (AnimIns)
	{
		switch (inFinger)
		{
		case EFingerInputType::XRGrasp:
			isTrigger ? AnimIns->PoseAlphaGrasp = inActionValue : 
				AnimIns->PoseAlphaGrasp = 0.0f;
			break;
		case EFingerInputType::XRIndex:
			isTrigger ? AnimIns->PoseAlphaIndexCurl = inActionValue:
				 AnimIns->PoseAlphaIndexCurl = 0.0f;
			break;
		case EFingerInputType::XRPoint:
			isTrigger ? AnimIns->PoseAlphaPoint = inActionValue :
				AnimIns->PoseAlphaPoint = 0.0f;
			break;
		case EFingerInputType::XRThumbUp:
			isTrigger ? AnimIns->PoseAlphaThumbUp = inActionValue :
				AnimIns->PoseAlphaThumbUp = 0.0f;
			break;
		}

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

	UAnimInstance* AnimIns = GetABPInstance();
	if (AnimIns) {
		ULeeXRAnimInstance* LeeAnimIns = Cast<ULeeXRAnimInstance>(AnimIns);
		if (LeeAnimIns)
		{
			LeeAnimIns->PoseAlphaThumbUp = 1.0f;
		}
		LeeScreenLog("Anim :%s", FColor::Cyan, *HandSkeletal->GetAnimInstance()->GetName());
	}
}

void ALeeXRHandController::SetFingerAnimationPose(USkeletalMeshComponent* inComponet, const FInputActionInstance& ActionInstance)
{
	if (inComponet == nullptr) {
		LeeScreenLog("Hand Skeletal is Null", FColor::Red);
		return;
	};
	LEE_SCOPE_CYCLE_COUNTER(ICTUController);

	ETriggerEvent inEventType = ActionInstance.GetTriggerEvent();
	float PoseValue = inEventType == ETriggerEvent::Started ? 1.0f : 0.0f;
	FString ActName = ActionInstance.GetSourceAction()->GetName();

	//ULeeXRAnimInstance* AnimIns = CastChecked<ULeeXRAnimInstance>(inComponet->GetAnimInstance());
	//if (!AnimIns) return;


	//if (ActName == IA_FingerPoint->GetName())
	//{
	//	AnimIns->PoseAlphaPoint = PoseValue;
	//}
	//else if (ActName == IA_CurlIndex->GetName())
	//{
	//	AnimIns->PoseAlphaIndexCurl = PoseValue;
	//}
	//else if (ActName == IA_HandThumpUp->GetName())
	//{
	//	AnimIns->PoseAlphaThumbUp = PoseValue;
	//}
	//else if (ActName == IA_Grasp->GetName())
	//{
	//	AnimIns->PoseAlphaGrasp = PoseValue;

	//	PoseValue == 1 ? 
	//		GraspObject() : GraspRelease();
	//}

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
		EnhancedInputComponent->BindAction(IA_Grasp, ETriggerEvent::Started, this, &ALeeXRHandController::OnFingerAnimation);
		EnhancedInputComponent->BindAction(IA_Grasp, ETriggerEvent::Completed, this, &ALeeXRHandController::OnFingerAnimation);
		EnhancedInputComponent->BindAction(IA_Grasp, ETriggerEvent::Canceled, this, &ALeeXRHandController::OnFingerAnimation);
		//EnhancedInputComponent->BindAction(IA_Grasp, ETriggerEvent::Triggered, this, &ALeeXRHandBase::OnHandTrigger);

		EnhancedInputComponent->BindAction(IA_HandThumpUp, ETriggerEvent::Started, this, &ALeeXRHandController::OnFingerAnimation);
		EnhancedInputComponent->BindAction(IA_HandThumpUp, ETriggerEvent::Canceled, this, &ALeeXRHandController::OnFingerAnimation);
		EnhancedInputComponent->BindAction(IA_HandThumpUp, ETriggerEvent::Completed, this, &ALeeXRHandController::OnFingerAnimation);

		EnhancedInputComponent->BindAction(IA_CurlIndex, ETriggerEvent::Started, this, &ALeeXRHandController::OnFingerAnimation);
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
