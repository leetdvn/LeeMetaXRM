// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/LeeXRGrabActors.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ALeeXRGrabActors::ALeeXRGrabActors()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	

	GrabRegion = CreateDefaultSubobject<UBoxComponent>(TEXT("GrabRegion"));
	SetRootComponent(GrabRegion);

	ActorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ActorMesh"));
	ActorMesh->SetupAttachment(GrabRegion);
}

// Called when the game starts or when spawned
void ALeeXRGrabActors::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALeeXRGrabActors::OnGrab(USkeletalMeshComponent* inComponent, FVector& InGrabLocation)
{
	if (inComponent)
	{
		switch (GrabType)
		{
		case EGrabType::EGT_Free: {
			
			inComponent->SetSimulatePhysics(false);
			bIsheld = ActorMesh->AttachToComponent(inComponent, FAttachmentTransformRules::KeepWorldTransform, NAME_None);
			if (bIsheld)
			{
				GrabBodySkeletal = inComponent;
			}
			break;
		}
		case EGrabType::EGT_Snap:
			break;
		case EGrabType::EGT_None:
			break;
		default:
			break;
		}
		//inComponent->SetSimulatePhysics(false);
		//inComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ALeeXRGrabActors::OnRelease(USkeletalMeshComponent* inComponent)
{
	if (inComponent)
	{
		switch (GrabType)
		{
		case EGrabType::EGT_Free: {

			if (bIsheld)
			{
				if (inComponent== GrabBodySkeletal) {
					ActorMesh->SetSimulatePhysics(true);
					bIsheld = false;
				}
			}
			break;
		}
		case EGrabType::EGT_Snap:
			break;
		case EGrabType::EGT_None:
			break;
		default:
			break;
		}
		//inComponent->SetSimulatePhysics(false);
		//inComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

// Called every frame
void ALeeXRGrabActors::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

