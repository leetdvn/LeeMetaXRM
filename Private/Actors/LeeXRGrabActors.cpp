// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/LeeXRGrabActors.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include <Definitions.h>

// Sets default values
ALeeXRGrabActors::ALeeXRGrabActors()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GrabRegion = CreateDefaultSubobject<UBoxComponent>(TEXT("GrabRegion"));

	//GrabRegion->SetupAttachment(ActorMesh);
	GrabRegion->SetupAttachment(ActorMesh);

	GrabRegion->SetCollisionProfileName(TEXT("Grabbable"));

}

// Called when the game starts or when spawned
void ALeeXRGrabActors::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALeeXRGrabActors::OnGrab(USkeletalMeshComponent* inComponent,const FVector& InGrabLocation)
{
	/*
	Need Setup Collision Profileing and Custom Edit Collision Presets
	*/
	if (inComponent == nullptr) return;

	FAttachmentTransformRules AttachRules = GrabType == EGrabType::EGT_Free ?
		FAttachmentTransformRules::KeepWorldTransform : 
		FAttachmentTransformRules::SnapToTargetNotIncludingScale;

	if (ActorMesh->IsSimulatingPhysics()) {
		ActorMesh->SetSimulatePhysics(false);
	}

	bIsheld = ActorMesh->AttachToComponent(inComponent, AttachRules, NAME_None);
	if (bIsheld) {
		GrabBodySkeletal = inComponent;
	}
}

void ALeeXRGrabActors::OnRelease(USkeletalMeshComponent* inComponent)
{
	if (inComponent == nullptr) return;

	if (bIsheld)
	{
		LeeScreenLog("Releasing Object %s", FColor::Green, *GrabBodySkeletal->GetName());
		if (inComponent == GrabBodySkeletal) {
			ActorMesh->SetSimulatePhysics(true);
			bIsheld = false;
		}
	}

	//switch (GrabType)
	//{
	//case EGrabType::EGT_Free: {

	//	break;
	//}
	//case EGrabType::EGT_Snap:
	//	break;
	//case EGrabType::EGT_None:
	//	break;
	//default:
	//	break;
	//}
}

// Called every frame
void ALeeXRGrabActors::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

