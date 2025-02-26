// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/LeeXRHandBase.h"
#include <Components/SkeletalMeshComponent.h>
#include <Components/SphereComponent.h>
#include <Components/ArrowComponent.h>
#include <Definitions.h>

// Sets default values
ALeeXRHandBase::ALeeXRHandBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController"));
	SetRootComponent(MotionController);

	WidgetInteraction = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteraction"));
	WidgetInteraction->SetupAttachment(MotionController);

	GrabSphere = CreateDefaultSubobject<USphereComponent>(TEXT("GrabSphereCollison"));
	GrabSphere->SetupAttachment(MotionController);

}

// Called when the game starts or when spawned
void ALeeXRHandBase::BeginPlay()
{
	Super::BeginPlay();
	
	LeeScreenLog("Hand %s Checking", FColor::Green,*GetName());
}

// Called when compiled
void ALeeXRHandBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	switch (HandType)
	{
	case EControllerHand::Left: {
		MotionController->MotionSource = TEXT("Left");
		break;
	}
	case EControllerHand::Right:
		MotionController->MotionSource = TEXT("Right");
		break;
	}
}

#if WITH_EDITOR
void ALeeXRHandBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.Property)
	{
		FName PropertyName = PropertyChangedEvent.Property->GetFName();
		if (PropertyName == GET_MEMBER_NAME_CHECKED(ALeeXRHandBase,ControllerType))
		{
			if (ControllerType == ELeeXRHandType::LeeXRHandTracking)
			{
				//Do Something With Hand Tracking Editor Mode
	
			}
			else {
				//Do Something With Hand Controller Editor Mode
			}
		}
	}
}
#endif

/// Grab Object
void ALeeXRHandBase::GraspObject()
{

	if (HandSkeletal == nullptr || 
		ControllerType == ELeeXRHandType::LeeXRHandTracking) return;

	TArray<AActor*> OverlappingActors;
	GrabSphere->GetOverlappingActors(OverlappingActors);

	if (!OverlappingActors.IsEmpty())
	{
		AActor* OverlappingActor = OverlappingActors[0];
		if (OverlappingActor)
		{
			LeeScreenLog("Grasp Object :%s", FColor::Green,*OverlappingActor->GetName());

			CurrentGrabObject = TScriptInterface<ILeeXRInteraction>(OverlappingActor);
			if (CurrentGrabObject) {
				bIsHeld = true;
				FVector GrabLocation = HandSkeletal->GetComponentLocation();
				CurrentGrabObject->OnGrab(HandSkeletal, GrabLocation);
				LeeScreenLog("Grabbing Object %s", FColor::Green, *OverlappingActor->GetName());

			}
		}
	}

}

/// Grab Release
void ALeeXRHandBase::GraspRelease()
{

	if (ControllerType == ELeeXRHandType::LeeXRHandTracking) return;

	if (CurrentGrabObject == nullptr ||
		HandSkeletal == nullptr) {
		
		return;
	}

	CurrentGrabObject->OnRelease(HandSkeletal);
	CurrentGrabObject = nullptr;
	bIsHeld = false;
}

// Called every frame
void ALeeXRHandBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Find Actor to Grab
AActor* ALeeXRHandBase::FindActorToGrab(TArray<AActor*> &inActors, FString inTag)
{
	for (auto Actor : inActors)
	{
		if (Actor->ActorHasTag(*inTag))
		{
			LeeScreenLog("Grabbing Object %s", FColor::Green, *Actor->GetName());
			return Actor;
		}
	}
	return nullptr;
}

