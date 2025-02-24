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

	GrabSphereCollison = CreateDefaultSubobject<USphereComponent>(TEXT("GrabSphereCollison"));
	GrabSphereCollison->SetupAttachment(MotionController);

}

// Called when the game starts or when spawned
void ALeeXRHandBase::BeginPlay()
{
	Super::BeginPlay();
	
	LeeScreenLog("Hand %s Checking", FColor::Green,*GetName());
}

// Called every frame
void ALeeXRHandBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

