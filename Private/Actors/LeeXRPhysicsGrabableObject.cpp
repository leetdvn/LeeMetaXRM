// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/LeeXRPhysicsGrabableObject.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Components/PrimitiveComponent.h"
#include "Components/StaticMeshComponent.h"
#include <Kismet/KismetMaterialLibrary.h>
#include "LeeXRUltils.h"
#include <APawn/LeeXRCharacter.h>

using namespace LeeXRUltils;
// Sets default values
ALeeXRPhysicsGrabableObject::ALeeXRPhysicsGrabableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ShapePhysicsMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ActorMesh"));
	ShapePhysicsMesh->SetupAttachment(RootComponent);

	//ShapeCollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShapeCollisionMesh"));
	//ShapeCollisionMesh->SetupAttachment(ShapePhysicsMesh);

}

void ALeeXRPhysicsGrabableObject::ConstructionEditor()
{
	if (!ShapePhysicsMesh->GetStaticMesh()) return;
	MaterialIns = UKismetMaterialLibrary::CreateDynamicMaterialInstance(0, ShapePhysicsMesh->GetMaterial(0), TEXT("LeeXRMaterialDynamic"));
	if (MaterialIns) {
		//UKismetMaterialLibrary::SetMaterialInstanceScalarParameterValue(MaterialIns, TEXT("OnOff"), 1.f);
		//UMaterialEditingLibrary::SetMaterialInstanceStaticSwitchParameterValue(MaterialIns, TEXT("OnOff"), true);
	}
}

// Called when the game starts or when spawned
void ALeeXRPhysicsGrabableObject::BeginPlay()
{
	Super::BeginPlay();
	

	//MaterialIns = ShapePhysicsMesh->CreateAndSetMaterialInstanceDynamic(0);
	ShapePhysicsMesh->BodyInstance.SetMassOverride(150.f);

}

void ALeeXRPhysicsGrabableObject::OnGrabObjects(UMotionControllerComponent* inComponent)
{
	Super::OnGrabObjects(inComponent);
}

void ALeeXRPhysicsGrabableObject::OnReleaseObjects(UMotionControllerComponent* inComponent)
{
	Super::OnReleaseObjects(inComponent);
	LeeScreenLog("Release %s", FColor::Green, *inComponent->GetName());
}

void ALeeXRPhysicsGrabableObject::InitSettings()
{
	Super::InitSettings();
	//Set the Collision Profile
	//ShapeCollisionMesh->SetCollisionProfileName(TEXT("Grabbable"));
}



// Called every frame
void ALeeXRPhysicsGrabableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

