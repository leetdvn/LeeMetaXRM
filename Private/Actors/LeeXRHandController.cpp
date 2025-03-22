// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/LeeXRHandController.h"
#include "Definitions.h"
#include <EnhancedInputComponent.h>
#include "UObject/ConstructorHelpers.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include <HeadMountedDisplayFunctionLibrary.h>


ALeeXRHandController::ALeeXRHandController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	HandSkeletal = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandSkeletal"));
	HandSkeletal->SetupAttachment(MotionController);

	//GrabSphere = CreateDefaultSubobject<USphereComponent>(TEXT("GrabSphereCollison"));
	//GrabSphere->SetupAttachment(HandSkeletal);
	//Set Init Hand Left or Right
	GrabsContraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("GrabsContraint"));
	GrabsContraint->SetupAttachment(MotionController);

	ControllerType = ELeeXRHandType::LeeXRController;

	//HandSkeletal->bBlendPhysics = true;

}

void ALeeXRHandController::SetPhysicsAllBodyBlendWeight(float inWeight)
{
	//Set Up Bone Physisc
	FString BoneName = HandType == EControllerHand::Right ? "hand_r" : "hand_l";
	if (HandSkeletal) {
		
		HandSkeletal->SetPhysicsBlendWeight(inWeight);
		HandSkeletal->SetAllBodiesBelowSimulatePhysics(*BoneName, true);
		HandSkeletal->SetAllBodiesBelowPhysicsBlendWeight(*BoneName, inWeight, false, true);
		HandSkeletal->AccumulateAllBodiesBelowPhysicsBlendWeight(*BoneName, .15f);
		LEE_LOG(LogLeeXRHandController, Log, "Set Physics All Body Blend Weight %f", inWeight);
	


	}
}

void ALeeXRHandController::OnGrabObject()
{
	Super::OnGrabObject();
	LEE_SCOPE_CYCLE_COUNTER(ICTUController);
}

void ALeeXRHandController::OnReleaseObject()
{
	Super::OnReleaseObject();
	LEE_SCOPE_CYCLE_COUNTER(ICTUController);
}

void ALeeXRHandController::BeginPlay()
{
	Super::BeginPlay();
	
	SetHandSwitch(HandType == EControllerHand::Left);

	LEE_SCOPE_CYCLE_COUNTER(ICTUController);
	
	ADDMEMORYSTAT(this, STAT_ICTUController);
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
	LEE_SCOPE_CYCLE_COUNTER(ICTUCharacter);
	
	
	ETriggerEvent TriggerEvent = ActionInstance.GetTriggerEvent();

	switch (TriggerEvent)
	{
	case ETriggerEvent::Triggered: {
		if (TeleportValid()) {
			bTeleportTraceActive = true;
			FVector StartPos = LeeXRGetWorldLocation(HandSkeletal);
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
	Super::SetFingerAnimationPose(inComponet, ActionInstance);

	LEE_SCOPE_CYCLE_COUNTER(ICTUController);
}

/// <summary>
/// Init Hand Setup
/// </summary>
void ALeeXRHandController::InittializeSetup()
{
	Super::InittializeSetup();

	if(HandSkeletal->DoesSocketExist(TEXT("Index3")))
		WidgetInteraction->SetupAttachment(HandSkeletal,TEXT("Index3"));

	//SetPhysicsAllBodyBlendWeight(BodyWeighted);
}

void ALeeXRHandController::InitPhysicSetup()
{

}
