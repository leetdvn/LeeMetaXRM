// Fill out your copyright notice in the Description page of Project Settings.


#include "APawn/LeeXRCharacter.h"

#include <Camera/CameraComponent.h>
#include <EnhancedInputComponent.h>
#include "Definitions.h"
#include <InputAction.h>
#include <EnhancedInputSubsystems.h>
#include <HeadMountedDisplayFunctionLibrary.h>
#include "Actors/LeeXRHandBase.h"
#include <Kismet/KismetSystemLibrary.h>
#include <LeeXRUltils.h>
#include <Actors/LeeXRHandController.h>
#include <Actors/LeeXRHandTracking.h>
#include <Animations/LeeXRAnimInstance.h>

using namespace LeeXRUltils;

// Sets default values
ALeeXRCharacter::ALeeXRCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	XROrigin = CreateDefaultSubobject<USceneComponent>(TEXT("LeeVROrigin"));
	XROrigin->SetupAttachment(GetMesh());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	Camera->SetupAttachment(XROrigin);

}

// Get the hand animation instance
UAnimInstance* ALeeXRCharacter::GetHandAnimInstance(bool isLeft)
{
	return  XRHandLeft->GetHandAnimInstance();
}

// Called when the game starts or when spawned
void ALeeXRCharacter::BeginPlay()
{
	HandInitialize();

	Super::BeginPlay();

	InitContext();

	//LeeXRInitMappingContext(this,DefaultMappingContext);
	//LeeXRInitMappingContext(this, HandMappingContext);

	//Set Tracking Origin to FLoor
	bool isEnable = UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled();

	if (isEnable)
	{
		UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Stage);

		UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), TEXT("vr.PixelDensity 1.0"));
	}


}

#if WITH_EDITOR
void ALeeXRCharacter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.Property)
	{
		FName PropertyName = PropertyChangedEvent.Property->GetFName();
		if (PropertyName == GET_MEMBER_NAME_CHECKED(ALeeXRCharacter, HandType))
		{
			if (HandType == ELeeXRHandType::LeeXRHandTracking)
			{
				//Do Something With Hand Tracking Editor Mode
			}

		}
	}
}
#endif


// Called every frame
void ALeeXRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALeeXRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Bind the action to the delegate
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ALeeXRCharacter::OnMoving);
		EnhancedInputComponent->BindAction(IA_GraspLeft, ETriggerEvent::Started, this, &ALeeXRCharacter::OnHandGrabing);
		EnhancedInputComponent->BindAction(IA_GraspLeft, ETriggerEvent::Completed, this, &ALeeXRCharacter::OnHandRelease);
		EnhancedInputComponent->BindAction(IA_GraspLeft, ETriggerEvent::Canceled, this, &ALeeXRCharacter::OnHandRelease);
		EnhancedInputComponent->BindAction(IA_GraspLeft, ETriggerEvent::Triggered, this, &ALeeXRCharacter::OnHandTrigger);


		EnhancedInputComponent->BindAction(IA_GraspRight, ETriggerEvent::Started, this, &ALeeXRCharacter::OnHandGrabing);
		EnhancedInputComponent->BindAction(IA_GraspRight, ETriggerEvent::Triggered, this, &ALeeXRCharacter::OnHandTrigger);
		EnhancedInputComponent->BindAction(IA_GraspRight, ETriggerEvent::Completed, this, &ALeeXRCharacter::OnHandRelease);
		EnhancedInputComponent->BindAction(IA_GraspRight, ETriggerEvent::Canceled, this, &ALeeXRCharacter::OnHandRelease);


	}

}

// Called when the player is moving
void ALeeXRCharacter::OnMoving()
{
	LeeScreenLog("Moving %s",FColor::Cyan,TEXT("TESTTTTT"));
}

// Called when the player is grabbing
void ALeeXRCharacter::OnHandGrabing(const FInputActionInstance& ActionInstance)
{
	FString ActName = ActionInstance.GetSourceAction()->GetName();
	bool isLeft = ActName.EndsWith("Left"); 
	TObjectPtr<ALeeXRHandBase> Hand = isLeft ?
		XRHandLeft :
		XRHandRight;

	if (Hand && Hand->IsValidControllerType(ELeeXRHandType::LeeXRController)) {
		Hand->GraspObject();
	}
	else {
		LeeScreenLog("Hand Not Found", FColor::Red);
	}

}

// Called when the player is releasing
void ALeeXRCharacter::OnHandRelease(const FInputActionInstance& ActionInstance)
{
	FString ActName = ActionInstance.GetSourceAction()->GetName();
	bool isLeft = ActName.EndsWith("Left");
	TObjectPtr<ALeeXRHandBase> Hand = isLeft ?
		XRHandLeft :
		XRHandRight;

	if (Hand && Hand->IsValidControllerType(ELeeXRHandType::LeeXRController)) {
		Hand->GraspRelease();
	}
	else {
		LeeScreenLog("Hand Not Found", FColor::Red);
	}
}

// Initialize the context
void ALeeXRCharacter::InitContext()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
			Subsystem->AddMappingContext(HandMappingContext, 0);
		}
	}
}

void ALeeXRCharacter::OnHandTrigger(const FInputActionInstance& ActionInstance)
{
	FString ActName = ActionInstance.GetSourceAction()->GetName();
	bool isLeft = ActName.EndsWith("Left");
	TObjectPtr<ALeeXRHandBase> Hand = isLeft ?
		XRHandLeft :
		XRHandRight;

	if (Hand) {
		///Play Animation
		ALeeXRHandController* HandControl = LeeXRGetBaseClass<ALeeXRHandController>(Hand);
		if (HandControl)
		{
			FInputActionValue ActValue = ActionInstance.GetValue();
			HandControl->PlayAnimAction(EFingerInputType::XRGrasp, ActValue.Get<float>(),true);

			LeeScreenLog("Triggering %f", FColor::Cyan, ActValue.Get<float>());
		}

	}
}

// Initialize the hands
void ALeeXRCharacter::HandInitialize()
{
	// Path Actor Blueprint
	///Hand Tracking /Script/Engine.Blueprint'/Game/BlueprintTemplates/Hands/BP_LeeXRHandTracking.BP_LeeXRHandTracking'
	FString PathRight = TEXT("/Game/BlueprintTemplates/Hands/BP_HandControllerRight.BP_HandControllerRight_C");  ////Script/Engine.Blueprint'/Game/BlueprintTemplates/Testing/BP_LeeXRHandController.BP_LeeXRHandController'
	FString PathLeft = TEXT("/Game/BlueprintTemplates/Hands/BP_HandControllerLeft.BP_HandControllerLeft_C");
	FAttachmentTransformRules AttachRules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;
	// Spawn the hands
	XRHandLeft = LeeXRSPawnActorBP<ALeeXRHandController>(this, PathLeft);

	XRHandRight = LeeXRSPawnActorBP<ALeeXRHandController>(this, PathRight);

	XRHandLeft->AttachToComponent(XROrigin, AttachRules);
	XRHandRight->AttachToComponent(XROrigin, AttachRules);


}

void ALeeXRCharacter::HandTrackingInitialize()
{

}



