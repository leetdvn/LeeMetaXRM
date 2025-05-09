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
	if (MaterialIns = ShapePhysicsMesh->CreateAndSetMaterialInstanceDynamic(0))
	{
		SetDisplayColor(StartColor);
	}

}

void ALeeXRPhysicsGrabableObject::SetDisplayColor(FColor inColor)
{
	if (MaterialIns)
	{
		MaterialIns->SetVectorParameterValue(FName(*MaterialParamName), inColor);
	}
}

void ALeeXRPhysicsGrabableObject::TurnOffRotation()
{
	if (auto World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(MeshRotationTimer);
	}

}

// Called when the game starts or when spawned
void ALeeXRPhysicsGrabableObject::BeginPlay()
{
	Super::BeginPlay();
	

	//MaterialIns = ShapePhysicsMesh->CreateAndSetMaterialInstanceDynamic(0);
	
	if (bMoveable) {
		if (auto World = GetWorld())
		{
			World->GetTimerManager().SetTimer(
				MeshRotationTimer,
				[this, World]() {
					//Rotate the mesh
					FRotator NewRot = ShapePhysicsMesh->GetComponentRotation();
					RotationAngle += RotationSpeed * World->GetDeltaSeconds();
					ShapePhysicsMesh->SetRelativeRotation(FRotator(NewRot.Pitch, bInverseRotation ? RotationAngle * -1 : RotationAngle ,NewRot.Roll));
				},
				0.1f, true);
		}
	}
}

void ALeeXRPhysicsGrabableObject::OnGrabObjects(UMotionControllerComponent* inComponent)
{
	Super::OnGrabObjects(inComponent);

	SetDisplayColor(FColor::Green);
}

void ALeeXRPhysicsGrabableObject::OnReleaseObjects(UMotionControllerComponent* inComponent)
{
	Super::OnReleaseObjects(inComponent);
	LeeScreenLog("Release %s", FColor::Green, *inComponent->GetName());

	//SetDisplayColor(StartColor);
}

void ALeeXRPhysicsGrabableObject::InitSettings()
{
	Super::InitSettings();
	//Set the Collision Profile
	//ShapeCollisionMesh->SetCollisionProfileName(TEXT("Grabbable"));
	ConstructionEditor();
}



void ALeeXRPhysicsGrabableObject::InitDynamicMaterial()
{
	if (MaterialIns == nullptr) {
		MaterialIns = ShapePhysicsMesh->CreateAndSetMaterialInstanceDynamic(0);
	}

	MaterialIns->SetVectorParameterValue(FName(*MaterialParamName), StartColor);
		//MaterialIns->SetVectorParameterValue(FName(*MaterialParamName), StartColor);

	LeeScreenLog("Material %s", FColor::Green, *MaterialParamName);
}

// Called every frame
void ALeeXRPhysicsGrabableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

