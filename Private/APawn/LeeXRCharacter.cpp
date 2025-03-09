// Fill out your copyright notice in the Description page of Project Settings.


#include "APawn/LeeXRCharacter.h"

#include <Camera/CameraComponent.h>
#include <EnhancedInputComponent.h>
#include <InputAction.h>
#include <EnhancedInputSubsystems.h>
#include <HeadMountedDisplayFunctionLibrary.h>
#include "Actors/LeeXRHandBase.h"
#include <Kismet/KismetSystemLibrary.h>
#include <LeeXRUltils.h>
#include <Actors/LeeXRHandController.h>
#include <Actors/LeeXRHandTracking.h>
#include <Animations/LeeXRAnimInstance.h>
#include <NavigationSystem.h>
#include <NiagaraDataInterfaceArrayFunctionLibrary.h>
#include <NiagaraComponent.h>
#include <NiagaraFunctionLibrary.h>
#include "Actors/LeeXRGrabbableActor.h"
#include "Actors/LeeXRHandPhysics.h"


using namespace LeeXRUltils;

DEFINE_STAT(STAT_ICTUCharacter);
DEFINE_STAT(STAT_ICTUCharacterMemory);


// Sets default values
ALeeXRCharacter::ALeeXRCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	LeeXROrigin = CreateDefaultSubobject<USceneComponent>(TEXT("LeeXROrigin"));
	LeeXROrigin->SetupAttachment(GetMesh());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	Camera->SetupAttachment(LeeXROrigin);

	DisplayMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DisplayMesh"));
	DisplayMesh->SetupAttachment(Camera);

}



// Get the hand animation instance
UAnimInstance* ALeeXRCharacter::GetHandAnimInstance(bool isLeft)
{
	LEE_SCOPE_CYCLE_COUNTER(ICTUCharacter);

	ALeeXRHandBase* Hand = isLeft ?
		XRHandLeft :
		XRHandRight;

	ALeeXRHandController* HandController = Cast<ALeeXRHandController>(Hand);
	if(IsValid(HandController))
		return  HandController->GetABPInstance();
	return nullptr;
}


void ALeeXRCharacter::CalculateMotionControllerVelocities()
{
	LEE_SCOPE_CYCLE_COUNTER(ICTUCharacter);

	//Calculate Left Hand Velocity
	LeftHandVelocity = XRHandLeft->GetMCLocationToWorld() - LastFrameLeftHandLocation;
	LastFrameLeftHandLocation = LeftHandVelocity;

	//Calculate Right Hand Velocity
	RightHandVelocity = XRHandRight->GetMCLocationToWorld() - LastFrameRightHandLocation;
	LastFrameRightHandLocation = RightHandVelocity;

}

void ALeeXRCharacter::UpdateClimbing()
{
	LEE_SCOPE_CYCLE_COUNTER(ICTUCharacter);

	FVector AccumulatedClimbVelocity = FVector::ZeroVector;

	if (IsValid(HeldLeftObject.Get()))
	{
		//Check if the player is climbing
		isClimbing = HeldLeftObject->ActorHasTag(TEXT("Climb"));
		
		AccumulatedClimbVelocity += FVector(0.0f, 0.0f, 0.0f);	// MCLeffVelocity
	}


	if (IsValid(HeldRightObject.Get()))
	{
		//Check if the player is climbing
		isClimbing = HeldRightObject->ActorHasTag(TEXT("Climb"));

		AccumulatedClimbVelocity += FVector(0.0f, 0.0f, 0.0f);	// MCLeffVelocity


		//GetCapsuleComponent()->AddWorldOffset(AccumulatedClimbVelocity*-1, true);

		LastFrameLeftHandLocation -= AccumulatedClimbVelocity;
		LastFrameRightHandLocation -= AccumulatedClimbVelocity;
		
	}
	//Check if the player is climbing


	UCapsuleComponent* Capsule = GetCapsuleComponent();
	
	Capsule->SetEnableGravity(!isClimbing);
	//ECollisionEnabled::Type CollisionType = isClimbing ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryAndPhysics;
	//Capsule->SetCollisionEnabled(CollisionType);

}

// Called when the game starts or when spawned
void ALeeXRCharacter::BeginPlay()
{
	XRHandLeft =  HandInitialize(HandType, true);
	XRHandRight =  HandInitialize(HandType, false);

	Super::BeginPlay();
	LEE_SCOPE_CYCLE_COUNTER(ICTUCharacter);

	ADDMEMORYSTAT(this, STAT_ICTUCharacterMemory);
	InitializationContext(GetWorld(), DefaultMappingContext);
	InitializationContext(GetWorld(), HandMappingContext, 1);

	//LeeXRInitMappingContext(this,DefaultMappingContext);
	//LeeXRInitMappingContext(this, HandMappingContext);

	//Set Tracking Origin to FLoor
	bool isEnable = UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled();

	if (isEnable)
	{
		UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::LocalFloor);

		UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), TEXT("vr.PixelDensity 1.0"));
	}

#if !UE_BUILD_SHIPPING
	//LeeScreenLog("Hand Type %s", FColor::Green, *HandType.ToString());
	UKismetSystemLibrary::ExecuteConsoleCommand(this, TEXT("ShowFlag.Collision"));
	UKismetSystemLibrary::ExecuteConsoleCommand(this, TEXT("Show collision"));
	UKismetSystemLibrary::ExecuteConsoleCommand(this, TEXT("STAT ICTUMV"));

#endif



}

void ALeeXRCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	LEE_SCOPE_CYCLE_COUNTER(ICTUCharacter);

	///Reset Profile Memories
	SET_MEMORY_STAT(STAT_ICTUCharacter, 0);
	SET_MEMORY_STAT(STAT_ICTUMV_TotalMemories, 0);
}

#if WITH_EDITOR
void ALeeXRCharacter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	LEE_SCOPE_CYCLE_COUNTER(ICTUCharacter);

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

	LEE_SCOPE_CYCLE_COUNTER(ICTUCharacter);
	///Climbing Option
	/*{
		CalculateMotionControllerVelocities();
		UpdateClimbing();
	}*/
}

// Called to bind functionality to input
void ALeeXRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	LEE_SCOPE_CYCLE_COUNTER(ICTUCharacter);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Bind the action to the delegate
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Started, this, &ALeeXRCharacter::OnMoving);
		//EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ALeeXRCharacter::OnMoving);

		//EnhancedInputComponent->BindAction(IA_GraspLeft, ETriggerEvent::Started, this, &ALeeXRCharacter::OnHandGrabing);
		//EnhancedInputComponent->BindAction(IA_GraspLeft, ETriggerEvent::Completed, this, &ALeeXRCharacter::OnHandRelease);
		//EnhancedInputComponent->BindAction(IA_GraspLeft, ETriggerEvent::Canceled, this, &ALeeXRCharacter::OnHandRelease);
		//EnhancedInputComponent->BindAction(IA_GraspLeft, ETriggerEvent::Triggered, this, &ALeeXRCharacter::OnHandTrigger);


		//EnhancedInputComponent->BindAction(IA_Grasp, ETriggerEvent::Started, this, &ALeeXRCharacter::OnHandGrabing);
		//EnhancedInputComponent->BindAction(IA_Grasp, ETriggerEvent::Completed, this, &ALeeXRCharacter::OnHandGrabing);
		//EnhancedInputComponent->BindAction(IA_Grasp, ETriggerEvent::Canceled, this, &ALeeXRCharacter::OnHandGrabing);

		//EnhancedInputComponent->BindAction(IA_RMenuInteract, ETriggerEvent::Started, this, &ALeeXRCharacter::OnHandInteract);
		//EnhancedInputComponent->BindAction(IA_RMenuInteract, ETriggerEvent::Canceled, this, &ALeeXRCharacter::OnHandInteract);
		//EnhancedInputComponent->BindAction(IA_RMenuInteract, ETriggerEvent::Completed, this, &ALeeXRCharacter::OnHandInteract);

	}

}

// Called when the player is moving
void ALeeXRCharacter::OnMoving()
{
	LEE_SCOPE_CYCLE_COUNTER(ICTUCharacter);


	//FVector StartPos = XRHandRight->GetMotionControllerLocation();
	//FVector ForwardVec = XRHandRight->GetMotionControllerForwardVector();

	//TeleportTrace(StartPos, ForwardVec);

	//UKismetSystemLibrary::DrawDebugSphere(this, ProjectedTeleportLocation, 10.0f, 12, FColor::Green, 5.0f, 0.0f);

	//LeeScreenLog("Location %s", FColor::Cyan,*ProjectedTeleportLocation.ToString());

}

/// <summary>
/// Grab Controller Input 
/// </summary>
/// <param name="ActionInstance"></param>
void ALeeXRCharacter::OnHandGrabing(const FInputActionInstance& ActionInstance)
{
	LEE_SCOPE_CYCLE_COUNTER(ICTUCharacter);
	FString ActName = ActionInstance.GetSourceAction()->GetName();

	ALeeXRHandController* HandController = Cast<ALeeXRHandController>(XRHandRight);

	if (!HandController) return;

	if (HandController->IsValidControllerType(ELeeXRHandType::LeeXRController))
		HandController->OnGrabObject();

	ULeeXRAnimInstance* LeeAnimIns = Cast<ULeeXRAnimInstance>(HandController->GetABPInstance());
	if (LeeAnimIns)
	{
		ETriggerEvent inEventType = ActionInstance.GetTriggerEvent();
		LeeAnimIns->PoseAlphaGrasp = inEventType == ETriggerEvent::Started ? 1.0f : 0.0f;
	}
}

// Initialize the hands
ALeeXRHandBase* ALeeXRCharacter::HandInitialize(ELeeXRHandType inType,bool isLeft)
{
	LEE_SCOPE_CYCLE_COUNTER(ICTUCharacter);
	if (DataLeft == nullptr || DataRight == nullptr)
	{
		LeeScreenLog("Hand Data Not Found", FColor::Red);
		return nullptr;
	}
	
	ULeeXRHandDataAsset* Data = isLeft ? DataLeft : DataRight;

	// Initialize the hand actor
	switch (inType)
	{
		case ELeeXRHandType::LeeXRController: {
			return InitializeHandActor<ALeeXRHandController>(Data->Assets.Controller);
		}
		case ELeeXRHandType::LeeXRHandTracking: {
			return InitializeHandActor<ALeeXRHandTracking>(Data->Assets.Tracking);
		}
		case ELeeXRHandType::LeeXRHandPhysics: {
			return InitializeHandActor<ALeeXRHandPhysics>(Data->Assets.Physics);
		}
	}
	return nullptr;

}



