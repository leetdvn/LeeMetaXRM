// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/LeeXRGrabComponent.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
// Sets default values for this component's properties
ULeeXRGrabComponent::ULeeXRGrabComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void ULeeXRGrabComponent::SetSholdSimulationOnDrop()
{
	UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(GetAttachParent());
	if (PrimComp && PrimComp->IsAnySimulatingPhysics())
	{
		bShouldSimulateOnDrop = true;
	}
}

void ULeeXRGrabComponent::SetPrimitiveComPhysics(bool bShouldSimulate)
{
	UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(GetAttachParent());
	if (PrimComp == nullptr) return;

	return PrimComp->SetSimulatePhysics(bShouldSimulate);
}

void ULeeXRGrabComponent::AttachParentToMotionController(UMotionControllerComponent* MotionController)
{
	
	if (!GetAttachParent()->AttachToComponent(MotionController, FAttachmentTransformRules::KeepWorldTransform, NAME_None))
	{
		FString displayName = GetAttachParent()->GetName();

		UE_LOG(LogTemp, Warning, TEXT("Attaching %s to %s"), *displayName,*MotionController->GetName());

		LeeScreenLog("Attaching %s to %s", FColor::Green, *displayName, *MotionController->GetName());
	}
}

bool ULeeXRGrabComponent::TryGrab(UMotionControllerComponent* MotionController)
{
	switch (GrabType)
	{
		case ELeeXRGrabType::LNONE: {

			break;
		}
		case ELeeXRGrabType::LFREE: {
			SetPrimitiveComPhysics(false);
			AttachParentToMotionController(MotionController);
			bIsHeld = true;
			break;
		}
		case ELeeXRGrabType::LSNAP: {
			SetPrimitiveComPhysics(false);
			AttachParentToMotionController(MotionController);
			bIsHeld = true;

			// Snap Grab bIsHeld = true;
			FRotator Inv = UKismetMathLibrary::InverseTransformRotation(GetAttachParent()->GetRelativeTransform(), MotionController->GetComponentTransform().GetRotation().Rotator());
			FHitResult* HitResult = new FHitResult();
			GetAttachParent()->SetRelativeRotation(Inv,false,HitResult,ETeleportType::TeleportPhysics);

			FVector newLocation = MotionController->K2_GetComponentToWorld().GetLocation() +
				(K2_GetComponentToWorld().GetLocation() - GetAttachParent()->K2_GetComponentToWorld().GetLocation()) * -1;

			GetAttachParent()->SetWorldLocation(newLocation,false,HitResult,ETeleportType::TeleportPhysics);
			break;
		}
		case ELeeXRGrabType::LCUSTOM: {
			bIsHeld = true;	
			break;
		}
	} 


	if (!bIsHeld) return false;

	///Call Event Dispatcher
	//OnGrab.Broadcast();
	
	MotionControllerRef = MotionController;
	
	APlayerController* Controller =	UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (Controller)
	{
		EControllerHand Select = MotionController->MotionSource == "Left" ? EControllerHand::Left : EControllerHand::Right;
		///Play Haptic Effect
		Controller->PlayHapticEffect(OnGrabHapticEffect, Select);
		return bIsHeld = true;
	}
	return false;
}

bool ULeeXRGrabComponent::TryRelease()
{
	if (GrabType == ELeeXRGrabType::LCUSTOM) { return bIsHeld = false; }

	if (GrabType == ELeeXRGrabType::LFREE || GrabType == ELeeXRGrabType::LSNAP)
	{
		if (bShouldSimulateOnDrop)
		{
			SetPrimitiveComPhysics(true);
		}

		DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		bIsHeld = false;
		
	}
	
	if (bIsHeld) return false;

	///Call Event Dispatcher
	//OnRelease.Broadcast();

	return true;
}


// Called when the game starts
void ULeeXRGrabComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	SetSholdSimulationOnDrop();

	///Set Collision Profile
	UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(GetAttachParent());
	if (PrimComp)
	{
		PrimComp->SetCollisionProfileName("PhysicsActor",true);	
	}
}


// Called every frame
void ULeeXRGrabComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULeeXRGrabComponent::UpdateGrabType()
{
	if (GrabType == ELeeXRGrabType::LNONE)
	{
		return;
	}


	switch (GrabType)
	{
		case ELeeXRGrabType::LNONE: { return; }
		case ELeeXRGrabType::LFREE:{
			// Free Grab
			// Implement your free grab logic here
			// ...
			break;
		}
		case ELeeXRGrabType::LSNAP:
			break;
		case ELeeXRGrabType::LCUSTOM: {
			// Custom Grab
			// Implement your custom grab logic here
			// ...
			break;
		}
	}
}

