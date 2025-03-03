// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/LeeXRSocketActor.h"
#include <Components/LeeXRMeshSocket.h>
#include <Definitions.h>
#include <Components/WidgetComponent.h>

// Sets default values
ALeeXRSocketActor::ALeeXRSocketActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletonBasic = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletonBasic"));
	SetRootComponent(SkeletonBasic);

	StatusAct = CreateDefaultSubobject<UWidgetComponent>(TEXT("StatusAct"));
	StatusAct->SetupAttachment(SkeletonBasic);

}

// Called when the game starts or when spawned
void ALeeXRSocketActor::BeginPlay()
{
	Super::BeginPlay();
	
	//TArray< ULeeXRMeshSocket*> MeshSockets{};
	//GetComponents<ULeeXRMeshSocket>(MeshSockets);
	//if (MeshSockets.IsEmpty()) return;

	//for (auto comp : MeshSockets)
	//{
	//	if (comp->OnComponentBeginOverlap.IsBound()) comp->OnComponentBeginOverlap.Clear();
	//	comp->OnComponentBeginOverlap.AddDynamic(this, &ALeeXRSocketActor::OnBeginOverlap);

	//	if (comp->OnComponentEndOverlap.IsBound()) comp->OnComponentEndOverlap.Clear();
	//	comp->OnComponentEndOverlap.AddDynamic(this, &ALeeXRSocketActor::OnEndOverlap);
	//}
}

void ALeeXRSocketActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (StatusAct = nullptr) return;

	LookAtComponent<UWidgetComponent>(StatusAct,true);
}

void ALeeXRSocketActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}