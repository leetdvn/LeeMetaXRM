// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/LeeXRSocketActor.h"
#include <Components/LeeXRMeshSocket.h>
#include <Definitions.h>
#include <Components/WidgetComponent.h>
#include <LeeXRUltils.h>
#include <APawn/LeeXRCharacter.h>

using namespace LeeXRUltils;

// Sets default values
ALeeXRSocketActor::ALeeXRSocketActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletonBasic = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletonBasic"));
	SetRootComponent(SkeletonBasic);

	PhysicsCollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PhysicsCollisionMesh"));
	PhysicsCollisionMesh->SetupAttachment(SkeletonBasic);

	StatusAct = CreateDefaultSubobject<UWidgetComponent>(TEXT("StatusAct"));
	StatusAct->SetupAttachment(SkeletonBasic);

}

// Called when the game starts or when spawned
void ALeeXRSocketActor::BeginPlay()
{
	Super::BeginPlay();
	
	InitSetting();

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

	if (StatusAct == nullptr) return;

	LookAtComponent<UWidgetComponent>(this,StatusAct,true);
}

void ALeeXRSocketActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void ALeeXRSocketActor::InitSetting()
{
	///Hiden Socket Setting Up base to Snap 
	SkeletonBasic->SetVisibility(false);
	SkeletonBasic->SetHiddenInGame(true);

	///Set the Collision Profile
	PhysicsCollisionMesh->Mobility = EComponentMobility::Static;
	//PhysicsCollisionMesh->SetCollisionProfileName(TEXT("Grabbable"));

	//Get the Character
	XRCharacter = LeeXRGetCustomCharacter<ALeeXRCharacter>(this);
	LEE_CHECK(XRCharacter);
}
