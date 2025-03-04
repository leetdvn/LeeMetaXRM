// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/LeeXRGrabbableActor.h"
#include <Kismet/KismetMathLibrary.h>

// Sets default values
ALeeXRGrabbableActor::ALeeXRGrabbableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ActorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ActorMesh"));
	SetRootComponent(ActorMesh);
}

void ALeeXRGrabbableActor::SetFreeze(bool bFreeze)
{
	FrezzeOnSnap = bFreeze;
}

FRotator ALeeXRGrabbableActor::GetTwoHandRotation(const  UMotionControllerComponent* FirstController, const  UMotionControllerComponent* SecondController)
{
	//Get the world location of the first and second controller
	FVector FirstWorldLoc = FirstController->GetComponentToWorld().GetLocation();
	//Get the world location of the first and second controller
	FVector SecondWorldLoc = SecondController->GetComponentToWorld().GetLocation();

	//Get the world rotation of the first controller
	FRotator FistWorldRot = FirstController->GetComponentToWorld().GetRotation().Rotator();
	//Get the world rotation of the second controller
	FRotator LookAt =  UKismetMathLibrary::FindLookAtRotation(FirstWorldLoc, SecondWorldLoc);
	//Combine the rotation of the first controller and the look at rotation
	FRotator CombineRotator = UKismetMathLibrary::ComposeRotators(LookAt, FRotator(0, 0, -90));

	return  FRotator(FistWorldRot.Roll, CombineRotator.Yaw, CombineRotator.Pitch * -1.f);
}

// Called when the game starts or when spawned
void ALeeXRGrabbableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

#if WITH_EDITOR
void ALeeXRGrabbableActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ALeeXRGrabbableActor, ActorMesh))
	{
		
	}

}
#endif
