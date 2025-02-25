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
	return isLeft ? XRHandLeft->GetHandAnimInstance() : XRHandRight->GetHandAnimInstance();
}

// Called when the game starts or when spawned
void ALeeXRCharacter::BeginPlay()
{
	HandInitialize();

	Super::BeginPlay();

	InitContext();

	//Set Tracking Origin to FLoor
	bool isEnable = UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled();

	if (isEnable)
	{
		UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Stage);

		UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), TEXT("vr.PixelDensity 1.0"));
	}


}

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
		EnhancedInputComponent->BindAction(IA_GrabLeft, ETriggerEvent::Started, this, &ALeeXRCharacter::OnHandGrabing);
		EnhancedInputComponent->BindAction(IA_GrabLeft, ETriggerEvent::Completed, this, &ALeeXRCharacter::OnHandRelease);
		EnhancedInputComponent->BindAction(IA_GrabLeft, ETriggerEvent::Canceled, this, &ALeeXRCharacter::OnHandRelease);

		EnhancedInputComponent->BindAction(IA_GrabRight, ETriggerEvent::Started, this, &ALeeXRCharacter::OnHandGrabing);
		EnhancedInputComponent->BindAction(IA_GrabRight, ETriggerEvent::Completed, this, &ALeeXRCharacter::OnHandRelease);
		EnhancedInputComponent->BindAction(IA_GrabRight, ETriggerEvent::Canceled, this, &ALeeXRCharacter::OnHandRelease);

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

	if (Hand) {
		Hand->GrabObject();
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

	if (Hand) {
		Hand->ReleaseObject();
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

// Initialize the hands
void ALeeXRCharacter::HandInitialize()
{

	// Path Actor Blueprint
	FString PathRight = TEXT("/Game/BlueprintTemplates/BP_LeeXRHandRight.BP_LeeXRHandRight_C");
	FString PathLeft = TEXT("/Game/BlueprintTemplates/BP_LeeXRHandLeft.BP_LeeXRHandLeft_C");
	FAttachmentTransformRules AttachRules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;
	// Spawn the hands
	XRHandLeft = LeeXRSPawnActorBP<ALeeXRHandBase>(this, PathLeft);
	XRHandRight = LeeXRSPawnActorBP<ALeeXRHandBase>(this, PathRight);

	XRHandLeft->AttachToComponent(XROrigin, AttachRules);
	XRHandRight->AttachToComponent(XROrigin, AttachRules);


}



