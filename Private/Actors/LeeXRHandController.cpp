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

}

void ALeeXRHandController::SetInputComponent()
{
	Super::SetInputComponent();

	UInputComponent* PlayerInputComponent = GetWorld()->GetFirstPlayerController()->InputComponent;
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
	}
	LEE_SCOPE_CYCLE_COUNTER(ICTUController);
}

/// <summary>
/// Init Hand Setup
/// </summary>
void ALeeXRHandController::InittializeSetup()
{
	Super::InittializeSetup();

	GrabSphere->AttachToComponent(HandSkeletal, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "palm_r");
	GrabSphere->SetRelativeLocation(FVector(0.0f, 2.5f, -2.5f));

}
