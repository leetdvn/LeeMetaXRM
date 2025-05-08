// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/LeeXRGrabActors.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include <Definitions.h>
#include "AbilitySystemComponent.h"


#define LOCATE_NAMESPACE "LeeMetaXRModules"

// Sets default values
ALeeXRGrabActors::ALeeXRGrabActors()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ActorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ActorMesh"));
	SetRootComponent(ActorMesh);

	ActorMesh->SetCollisionProfileName(TEXT("Grabbable"));

	AbilityComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

void ALeeXRGrabActors::SetCustomDepth(bool bEnable)
{
	if (ActorMesh) {
		ActorMesh->SetRenderCustomDepth(bEnable);

		int32 StencilValue = bEnable ? 1 : 0;
		ActorMesh->SetCustomDepthStencilValue(StencilValue);
		bIsCustomDepth = bEnable;
	}
}

// Called when the game starts or when spawned
void ALeeXRGrabActors::BeginPlay()
{
	Super::BeginPlay();
	
	//GrabRegion->OnComponentBeginOverlap.AddDynamic(this, &ALeeXRGrabActors::OnBeginOverlap);

	AbilityComponent->InitAbilityActorInfo(this, this);

	LEE_SCOPE_CYCLE_COUNTER(LeeXRGrabable);
	
	ADDMEMORYSTAT(this, STAT_LeeXRGrabable);


	//AbilityComponent->AddGameplayCue(ObjectTags);
}

void ALeeXRGrabActors::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	SET_MEMORY_STAT(STAT_LeeXRGrabable, 0);
}


void ALeeXRGrabActors::OnGrabObjects(UMotionControllerComponent* inComponent)
{
	Super::OnGrabObjects(inComponent);

}

void ALeeXRGrabActors::OnReleaseObjects(UMotionControllerComponent* inComponent)
{
	Super::OnReleaseObjects(inComponent);

}

void ALeeXRGrabActors::InitSettings()
{
	Super::InitSettings();
	//Set the Collision Profile

}

void ALeeXRGrabActors::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	//Set the Collision Profile
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
		//LeeScreenLog("Overlap %s", FColor::Green, *OtherActor->GetName());
	}
}


#undef LOCATE_NAMESPACE