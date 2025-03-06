// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/LeeXRGrabbableActor.h"
#include <MotionControllerComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <SphereComponent.h>

// Sets default values
ALeeXRGrabbableActor::ALeeXRGrabbableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	FRotator CombineRotator = UKismetMathLibrary::ComposeRotators(LookAt, SecondaryOffset);
	return  FRotator(FistWorldRot.Roll, CombineRotator.Yaw, CombineRotator.Pitch*-1);
}

bool ALeeXRGrabbableActor::TryFindHandMeshOnController(UMotionControllerComponent* inController, USkeletalMeshComponent*& outMesh)
{
	TArray<USceneComponent*> Components{};
	inController->GetChildrenComponents(true, Components);

	if (Components.IsEmpty()) return false;

	for (auto Child : Components)
	{
		USkeletalMeshComponent* SkeletalMesh = Cast<USkeletalMeshComponent>(Child);
		if (SkeletalMesh) {
			outMesh = SkeletalMesh;
			return IsValid(outMesh);
		}
	}
	return false;
}

void ALeeXRGrabbableActor::CaptureHandMesh(UMotionControllerComponent* inController,bool isLeft)
{
	bool isSnapHand = TryFindHandMeshOnController(inController, HandSkeletalMesh);

	//Link Anim Layers Blueprint
	
	if (IsValid(AnimLayerClimb)) {
		HandSkeletalMesh->LinkAnimClassLayers(AnimLayerClimb.Get());
	}


	if (SnapHandMesh) {
		CacheHandTransform = HandSkeletalMesh->GetRelativeTransform();
		
		
		//Required to snap the Socket name to the Hand Mesh
		if (Sockets.MainSocketName.IsEmpty()) {
			FString msg = __FUNCTION__;
			UE_LOG(LogTemp, Warning, TEXT("Socket Name is Empty %s"), *msg);
		}
		HandSkeletalMesh->K2_AttachToComponent(HandSkeletalMesh->GetAttachParent(), *Sockets.MainSocketName, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
	}
		
}

void ALeeXRGrabbableActor::ReleaseHandMesh(UMotionControllerComponent*& inController, bool isLeft)
{
	//UnLink Anim Layers Blueprint
	if (IsValid(AnimLayerClimb)) {
		HandSkeletalMesh->UnlinkAnimClassLayers(AnimLayerClimb.Get());
	}


	if (SnapHandMesh) {
		//Release the Hand Mesh
				//Required to snap the Socket name to the Hand Mesh
		if (Sockets.MainSocketName.IsEmpty()) {
			FString func =  __FUNCTION__;

			LEE_LOG(LeeXRMacro, Warning, "Socket Name is Empty %s", *func);
		}

		HandSkeletalMesh->K2_AttachToComponent(inController, *Sockets.MainSocketName, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);

		HandSkeletalMesh->SetRelativeTransform(CacheHandTransform);
		//HandSkeletalMesh->SetRelativeTransform(CacheHandTransform);
	}
}

// Called when the game starts or when spawned
void ALeeXRGrabbableActor::BeginPlay()
{
	Super::BeginPlay();	
	
	InitSettings();
}

void ALeeXRGrabbableActor::OnGrab(UObject* inComponent, const FVector& InGrabLocation)
{
}

void ALeeXRGrabbableActor::OnRelease(UObject* inComponent)
{
}

void ALeeXRGrabbableActor::OnGrabObjects(UMotionControllerComponent* inComponent)
{
	LeeScreenLog("Abstract Act Grabs ", FColor::Green);
	LeeXROnGrabObject.Broadcast();

	//Player Haptic Feedback
	LeeXRPlayerHapicEffect(this, HapticEffect,inComponent->GetTrackingSource());
}

void ALeeXRGrabbableActor::OnReleaseObjects(UMotionControllerComponent* inComponent)
{
	LeeScreenLog("Abstract Act Release ", FColor::Green);
	LeeXROnReleaseObject.Broadcast();

}

void ALeeXRGrabbableActor::InitSettings()
{
	//Do Nothing
}



#if WITH_EDITOR


void ALeeXRGrabbableActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	
	if (PROPERTYCHANGED(ALeeXRGrabbableActor,GrabableType))
	{
		InitSettings();
	}

}
#endif
