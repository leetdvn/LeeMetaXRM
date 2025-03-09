// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/LeeXRHandPhysics.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

ALeeXRHandPhysics::ALeeXRHandPhysics(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	HandSkeletal = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandMesh"));
	HandSkeletal->SetupAttachment(MotionController);

	CubeConstraint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeConstraint"));
	CubeConstraint->SetupAttachment(MotionController);

	//PhysicContraint->SetConstrainedComponents( CubeConstraint, NAME_None, HandSkeletal,NAME_None);
}

void ALeeXRHandPhysics::InittializeSetup()
{
	Super::InittializeSetup();
}
