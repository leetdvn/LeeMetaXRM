// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/LeeXRHandController.h"
#include "Definitions.h"

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
	FString HandName = isLeft ? "Left" : "Right";

	MotionController->MotionSource = FName(*HandName);
}

void ALeeXRHandController::PlayAnimAction(const EFingerInputType& inFinger,float inActionValue, bool isTrigger)
{
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

}

void ALeeXRHandController::GraspRelease()
{
	Super::GraspRelease();
}




void ALeeXRHandController::BeginPlay()
{
	Super::BeginPlay();
	
	InittializeSetup();
}

/// <summary>
/// Init Hand Setup
/// </summary>
void ALeeXRHandController::InittializeSetup()
{
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
