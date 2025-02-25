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

// Called when the game starts or when spawned
void AICTUGrabbableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

