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


UAnimInstance* ALeeXRCharacter::GetHandAnimInstance(bool isLeft)
{

	return isLeft ? XRHandLeft->GetHandAnimInstance() : XRHandRight->GetHandAnimInstance();
}

// Called when the game starts or when spawned
void ALeeXRCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitContext();

	//Set Tracking Origin to FLoor
	bool isEnable = UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled();

	if (isEnable)
	{
		UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Stage);

		UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), TEXT("vr.PixelDensity 1.0"));
	}

	HandInitialize();

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
		EnhancedInputComponent->BindAction(IA_GrabLeft, ETriggerEvent::Started, this, &ALeeXRCharacter::OnActionGrab);
		EnhancedInputComponent->BindAction(IA_GrabLeft, ETriggerEvent::Completed, this, &ALeeXRCharacter::OnActionGrab);
		EnhancedInputComponent->BindAction(IA_GrabLeft, ETriggerEvent::Canceled, this, &ALeeXRCharacter::OnActionGrab);

		EnhancedInputComponent->BindAction(IA_GrabRight, ETriggerEvent::Started, this, &ALeeXRCharacter::OnActionGrab);
		EnhancedInputComponent->BindAction(IA_GrabRight, ETriggerEvent::Completed, this, &ALeeXRCharacter::OnActionGrab);
		EnhancedInputComponent->BindAction(IA_GrabRight, ETriggerEvent::Canceled, this, &ALeeXRCharacter::OnActionGrab);

	}

}

void ALeeXRCharacter::OnMoving()
{
	LeeScreenLog("Moving %s",FColor::Cyan,TEXT("TESTTTTT"));
}

void ALeeXRCharacter::OnActionGrab(const FInputActionInstance& ActionInstance)
{
	FString ActName = ActionInstance.GetSourceAction()->GetName();

	LeeScreenLog("Hand %s", FColor::Magenta, *ActName);

	return OnGrabObjects(ActionInstance, ActName.EndsWith("Right"));
}

void ALeeXRCharacter::InitContext()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
			Subsystem->AddMappingContext(HandMappingContext, 0);
			//Subsystem->AddMappingContext(HandMappingContext.LoadSynchronous(), 0);
		}
	}

}

void ALeeXRCharacter::OnGrabObjects(const FInputActionInstance& ActionInstance,bool isLeft)
{
	TObjectPtr<ALeeXRHandBase> Hand = isLeft ?
		XRHandLeft :
		XRHandRight;

	ETriggerEvent TriggerEvent = ActionInstance.GetTriggerEvent();

	if (TriggerEvent == ETriggerEvent::Canceled ||
		TriggerEvent == ETriggerEvent::Completed)
	{
		Hand->ReleaseObject();
	}
	else if (TriggerEvent == ETriggerEvent::Started)
	{
		Hand->GrabObject();

	}

	//FString TriggerEventName;
	//switch (TriggerEvent)
	//{
	//case ETriggerEvent::Started:
	//	TriggerEventName = "Started";
	//	break;
	//case ETriggerEvent::Ongoing:
	//	TriggerEventName = "Ongoing";
	//	break;
	//case ETriggerEvent::Canceled:
	//	TriggerEventName = "Canceled";
	//	break;
	//case ETriggerEvent::Completed:
	//	TriggerEventName = "Completed";
	//	break;
	//case ETriggerEvent::Triggered:
	//	TriggerEventName = "Triggered";
	//	break;
	//default:
	//	TriggerEventName = "None";
	//	break;
	//}
	//XRHandLeft->GrabObject();
}

void ALeeXRCharacter::HandInitialize()
{

	//GetWorld()->SpawnActor<AProjectile>(Location, Rotation, SpawnInfo);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	// Define the spawn location and rotation
	FVector SpawnLocation = FVector::ZeroVector;
	FRotator SpawnRotation = FRotator::ZeroRotator;

	UClass* HandClass = StaticLoadClass(ALeeXRHandBase::StaticClass(), this, TEXT("/Game/BlueprintTemplates/BP_LeeXRHandRight.BP_LeeXRHandRight_C"));
	//if (HandClass)
	//{
		// Spawn the left hand
	XRHandRight = GetWorld()->SpawnActor<ALeeXRHandBase>(HandClass, SpawnLocation, SpawnRotation, SpawnParams);
	if (!XRHandRight) return;

	XRHandRight->AttachToComponent(XROrigin, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	
	// Spawn the left hand
	//XRHandLeft = GetWorld()->SpawnActor<ALeeXRHandBase>(ALeeXRHandBase::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
	//if (XRHandLeft)
	//{
	//	XRHandLeft->AttachToComponent(XROrigin, FAttachmentTransformRules::KeepWorldTransform);
	//}

	//if(
	//XRHandLeft->AttachToComponent(XROrigin,FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	//XRHandRight = GetWorld()->SpawnActor<ALeeXRHandBase>(SpawnLocation, SpawnRotation, SpawnParams);

	//XRHandRight->AttachToComponent(XROrigin, FAttachmentTransformRules::KeepWorldTransform);

	//LeeScreenLog("Hand Initialize %s", FColor::Cyan, TEXT("Spawn Runing"));	
}



