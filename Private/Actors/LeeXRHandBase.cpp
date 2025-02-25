// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/LeeXRHandBase.h"
#include "MotionControllerComponent.h"
#include <Components/WidgetInteractionComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include <Components/SphereComponent.h>
#include <Components/ArrowComponent.h>
#include <LeeMetaXRM/Common/Definitions.h>

// Sets default values
ALeeXRHandBase::ALeeXRHandBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController"));
	SetRootComponent(MotionController);

	HandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandMesh"));
	HandMesh->SetupAttachment(MotionController);

	WidgetInteraction = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteraction"));
	WidgetInteraction->SetupAttachment(HandMesh);

	GrabSphere = CreateDefaultSubobject<USphereComponent>(TEXT("GrabSphereCollison"));
	GrabSphere->SetupAttachment(MotionController);

}

// Grab Object
void ALeeXRHandBase::GrabObject()
{
	LeeScreenLog("Grabbing Object", FColor::Green);

	TArray<AActor*> OverlappingActors;
	GrabSphere->GetOverlappingActors(OverlappingActors);

	if (!OverlappingActors.IsEmpty())
	{
		AActor* OverlappingActor = OverlappingActors[0];
		if (OverlappingActor)
		{
			//AActor* GrabAct= FindActorToGrab(OverlappingActors, "Grabbable");

			CurrentGrabObject = TScriptInterface<ILeeXRInteraction>(OverlappingActor);
			if (CurrentGrabObject) {
				bIsHeld = true;
				FVector GrabLocation = HandMesh->GetComponentLocation();
				CurrentGrabObject->OnGrab(HandMesh, GrabLocation);
				LeeScreenLog("Grabbing Object %s", FColor::Green, *OverlappingActor->GetName());

			}
		}
	}
}

// Release Object
void ALeeXRHandBase::ReleaseObject()
{
	if (!CurrentGrabObject)	return;

	CurrentGrabObject->OnRelease(HandMesh);
	CurrentGrabObject = nullptr;
	//UpdateOverlaps(true);
	//UpdateDefaultConfigFile();
	bIsHeld = false;
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

