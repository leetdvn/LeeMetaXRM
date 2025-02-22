// Fill out your copyright notice in the Description page of Project Settings.


#include "APawn/LeeXRPawn.h"
#include <EnhancedInputComponent.h>
#include <Camera/CameraComponent.h>
#include <MotionControllerComponent.h>
#include <VRNotificationsComponent.h>

// Sets default values
ALeeXRPawn::ALeeXRPawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VROrigin = CreateDefaultSubobject<USceneComponent>(TEXT("LeeVROrigin"));
	RootComponent = VROrigin;

	LCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("XRCamera"));
	LCamera->SetupAttachment(RootComponent);
	LeftGrip = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MC_LeftGrip"));
	LeftGrip->SetupAttachment(RootComponent);
	RightGrip = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MC_RightGrip"));
	RightGrip->SetupAttachment(RootComponent);
	LeftAim = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MC_LeftAim"));
	LeftAim->SetupAttachment(RootComponent);
	RightAim = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MC_RightAim"));
	RightAim->SetupAttachment(RootComponent);

	Notify = CreateDefaultSubobject<UVRNotificationsComponent>(TEXT("VRNotification"));
}

void ALeeXRPawn::StartTeleportTrace()
{
	UE_LOG(LeeXRCharacters, Warning, TEXT("Teleport Trace Started"));

}

void ALeeXRPawn::IAMove()
{
	///Trace Teleport


}

void ALeeXRPawn::IAMoveStart()
{
}

void ALeeXRPawn::IAMoveComplete()
{
}

void ALeeXRPawn::IATurn()
{
}

void ALeeXRPawn::IAGrabLeft()
{
}

void ALeeXRPawn::IAGrabRight()
{
}

void ALeeXRPawn::IALMenuToogle()
{
}

// Called when the game starts or when spawned
void ALeeXRPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALeeXRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALeeXRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		// Bind the action to the delegate
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ALeeXRPawn::IAMove);

		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Started, this, &ALeeXRPawn::IAMoveStart);

		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Completed, this, &ALeeXRPawn::IAMoveComplete);

		EnhancedInputComponent->BindAction(IA_Turn, ETriggerEvent::Started, this, &ALeeXRPawn::IATurn);

		EnhancedInputComponent->BindAction(IA_GrabLeft, ETriggerEvent::Started, this, &ALeeXRPawn::IAGrabLeft);

		EnhancedInputComponent->BindAction(IA_GrabRight, ETriggerEvent::Started, this, &ALeeXRPawn::IAGrabRight);

		EnhancedInputComponent->BindAction(IA_LMenuToogle, ETriggerEvent::Started, this, &ALeeXRPawn::IALMenuToogle);

	}
}

