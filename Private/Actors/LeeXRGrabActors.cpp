// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/LeeXRGrabActors.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include <Definitions.h>
#include "AbilitySystemComponent.h"


// Sets default values
ALeeXRGrabActors::ALeeXRGrabActors()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GrabRegion = CreateDefaultSubobject<UBoxComponent>(TEXT("GrabRegion"));

	//GrabRegion->SetupAttachment(ActorMesh);
	GrabRegion->SetupAttachment(ActorMesh);

	GrabRegion->SetCollisionProfileName(TEXT("Grabbable"));

	AbilityComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

// Called when the game starts or when spawned
void ALeeXRGrabActors::BeginPlay()
{
	Super::BeginPlay();
	
	//GrabRegion->OnComponentBeginOverlap.AddDynamic(this, &ALeeXRGrabActors::OnBeginOverlap);

	AbilityComponent->InitAbilityActorInfo(this, this);

	//AbilityComponent->AddGameplayCue(ObjectTags);
}

void ALeeXRGrabActors::OnGrab(UObject* inComponent,const FVector& InGrabLocation)
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

	bIsheld = ActorMesh->AttachToComponent(Cast<USceneComponent>(inComponent), AttachRules, NAME_None);
	if (bIsheld) {
		GrabUObject = inComponent;
	}
}

void ALeeXRGrabActors::OnRelease(UObject* inComponent)
{
	if (inComponent == nullptr) return;

	if (bIsheld)
	{
		//LeeScreenLog("Releasing Object %s", FColor::Green, *GrabUObject->GetName());
		if (inComponent == GrabUObject) {
			ActorMesh->SetSimulatePhysics(true);
			this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
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

void ALeeXRGrabActors::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (SweepResult.HasValidHitObjectHandle())
	{
		LeeScreenLog("Overlap %s", FColor::Green, *OtherActor->GetName());
	}
}

