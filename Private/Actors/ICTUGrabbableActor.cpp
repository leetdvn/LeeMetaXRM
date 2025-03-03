// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ICTUGrabbableActor.h"

// Sets default values
AICTUGrabbableActor::AICTUGrabbableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ActorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ActorMesh"));
	SetRootComponent(ActorMesh);
}

void AICTUGrabbableActor::SetFreeze(bool bFreeze)
{
	isFreeze = bFreeze;
}

// Called when the game starts or when spawned
void AICTUGrabbableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

#if WITH_EDITOR
void AICTUGrabbableActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(AICTUGrabbableActor, ActorMesh))
	{
		
	}

}
#endif
