// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/LeeXRGrabbableActor.h"
#include <MotionControllerComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <SphereComponent.h>
#include <Actors/LeeXRHandBase.h>
#include "PhysicsEngine/PhysicsConstraintComponent.h"


DEFINE_STAT(STAT_LeeXRGrabable);
DEFINE_STAT(STAT_LeeXRGrabableMemory);

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
	LEE_SCOPE_CYCLE_COUNTER(LeeXRGrabable);
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
	LEE_SCOPE_CYCLE_COUNTER(LeeXRGrabable);

	bool isSnapHand = TryFindHandMeshOnController(inController, HandSkeletalMeshRef);

	//Link Anim Layers Blueprint
	
	if (IsValid(AnimLayerClimb)) {
		HandSkeletalMeshRef->LinkAnimClassLayers(AnimLayerClimb.Get());
	}


	if (SnapHandMesh) {
		CacheHandTransform = HandSkeletalMeshRef->GetRelativeTransform();

		//Required to snap the Socket name to the Hand Mesh
		if (Sockets.MainSocketName.IsEmpty()) {
			FString msg = __FUNCTION__;
			UE_LOG(LogTemp, Warning, TEXT("Socket Name is Empty %s"), *msg);
		}
		HandSkeletalMeshRef->K2_AttachToComponent(
			HandSkeletalMeshRef->GetAttachParent(),
			*Sockets.MainSocketName,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld, true);
	}
		
}

void ALeeXRGrabbableActor::ReleaseHandMesh(UMotionControllerComponent*& inController, bool isLeft)
{
	LEE_SCOPE_CYCLE_COUNTER(LeeXRGrabable);
	//UnLink Anim Layers Blueprint
	if (IsValid(AnimLayerClimb)) {
		HandSkeletalMeshRef->UnlinkAnimClassLayers(AnimLayerClimb.Get());
	}


	if (SnapHandMesh) {
		//Release the Hand Mesh
				//Required to snap the Socket name to the Hand Mesh
		if (Sockets.MainSocketName.IsEmpty()) {
			FString func =  __FUNCTION__;

			LEE_LOG(LeeXRMacro, Warning, "Socket Name is Empty %s", *func);
		}

		HandSkeletalMeshRef->K2_AttachToComponent(
			inController,
			*Sockets.MainSocketName, 
			EAttachmentRule::KeepWorld,
			EAttachmentRule::KeepWorld,
			EAttachmentRule::KeepWorld, true);

		HandSkeletalMeshRef->SetRelativeTransform(CacheHandTransform);
		//HandSkeletalMesh->SetRelativeTransform(CacheHandTransform);
	}
}

void ALeeXRGrabbableActor::PhysicsContraintImplementation(UPhysicsConstraintComponent* inPhysicsContraint, ALeeXRHandBase*& inHandSkeletal)
{
	if (inPhysicsContraint == nullptr ) return ;

	if (!IsValid(PhysicsContraintRef))  PhysicsContraintRef = inPhysicsContraint;

	if (PhysicsContraintRef && PhysicsContraintRef != inPhysicsContraint) PhysicsContraintRef->BreakConstraint();

	PhysicsContraintRef = inPhysicsContraint;

	HandSkeletalMeshRef = inHandSkeletal->GetHandSkeletal();


	FString HandSocket = inHandSkeletal->GetHandType() == EControllerHand::Left ? "hand_l" : "hand_r";

	USkeletalMeshComponent* SkeletalComp = FindComponentByClass<USkeletalMeshComponent>();
	UStaticMeshComponent* StaticMeshComp = FindComponentByClass<UStaticMeshComponent>();
	LEE_LOG(LeeXRMacro, Log, "Static Mesh %s", *StaticMeshComp->GetName());

	if (StaticMeshComp)
		PhysicsContraintRef->SetConstrainedComponents(HandSkeletalMeshRef, *HandSocket, StaticMeshComp, NAME_None);
	else if (SkeletalComp) {
		PhysicsContraintRef->SetConstrainedComponents(HandSkeletalMeshRef, *HandSocket, SkeletalComp, NAME_None);
	}

	GetWorld()->GetTimerManager().SetTimer(TimerWeighted, this, &ALeeXRGrabbableActor::DetachWhenHandThresholdExceed, 0.02f, true);


}

void ALeeXRGrabbableActor::DetachWhenHandThresholdExceed()
{
	LEE_SCOPE_CYCLE_COUNTER(LeeXRGrabable);
	FVector HMeshLoc{};
	if(USkeletalMeshComponent* Skeletal = FindComponentByClass<USkeletalMeshComponent>())
		HMeshLoc = LeeXRGetWorldLocation(Skeletal);
	else if (UStaticMeshComponent* meshcomp = FindComponentByClass<UStaticMeshComponent>())
	{
		HMeshLoc = LeeXRGetWorldLocation(meshcomp);
	}

	if (MainControllerRef) {
		FVector MCloc = LeeXRGetWorldLocation(MainControllerRef);

		float Distance = FVector::Dist(HMeshLoc, MCloc);

		if (Distance > PhysicsGrabThreshold) {
			OnReleaseObjects(MainControllerRef);
		}
	}


}


// Called when the game starts or when spawned
void ALeeXRGrabbableActor::BeginPlay()
{
	Super::BeginPlay();	

	LEE_SCOPE_CYCLE_COUNTER(LeeXRGrabable);
	INC_MEMORY_STAT_BY(STAT_LeeXRGrabableMemory, this->GetResourceSizeBytes(EResourceSizeMode::EstimatedTotal));

	//Settitng Init When Game Starts
	InitSettings();
}

void ALeeXRGrabbableActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	SET_MEMORY_STAT(STAT_LeeXRGrabableMemory, 0);
}

void ALeeXRGrabbableActor::OnGrab(UObject* inComponent, const FVector& InGrabLocation)
{
}

void ALeeXRGrabbableActor::OnRelease(UObject* inComponent)
{

	//if (inComponent == nullptr || FrezzeOnSnap) return;

	//if (bIsheld)
	//{
	//	//LeeScreenLog("Releasing Object %s", FColor::Green, *GrabUObject->GetName());
	//	if (inComponent == MainControllerRef) {

	//		//Execute Release Delegate
	//		ActorMesh->SetSimulatePhysics(true);
	//		this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	//		bIsheld = false;
	//	}
	//}
}

void ALeeXRGrabbableActor::OnGrabObjects(UMotionControllerComponent* inComponent)
{
	LEE_SCOPE_CYCLE_COUNTER(LeeXRGrabable);

	ALeeXRHandBase* HandBase = inComponent->GetOwner<ALeeXRHandBase>();
	if (!HandBase) return;
	
	//GrabableType == ELeeXRGrabableType::LeeXROneHand ?
	//	OnGrab(HandBase->GetHandSkeletal(), HandBase->GetHandSkeletal()->GetComponentLocation()) :
	//	OnGrabObjects(inComponent);

	switch (GrabType)
	{
		case EGrabType::EGT_Free: {
			FreeGrababled(inComponent);
		}
		case EGrabType::EGT_Snap: {
			break;
		}
		case EGrabType::EGT_Weighted: {

			PhysicsContraintImplementation(HandBase->GetPhysicsConstraint(), HandBase);
			FreeGrababled(inComponent);

			break;
		}
	}
	///Check if the Physics Constraint is Valid


	//LeeXRPlayerHapicEffect(this,HapticEffect,inComponent->GetTrackingSource());
}

void ALeeXRGrabbableActor::OnReleaseObjects(UMotionControllerComponent* inComponent)
{
	LEE_SCOPE_CYCLE_COUNTER(LeeXRGrabable);

	///Release Object
	if (inComponent == nullptr || FrezzeOnSnap) return;

	switch (GrabType)
	{
	case EGrabType::EGT_Free: { break; }
	case EGrabType::EGT_Snap:
		break;
	case EGrabType::EGT_Weighted: {
		if (TimerWeighted.IsValid()) GetWorld()->GetTimerManager().ClearTimer(TimerWeighted);

		break;
	}
	case EGrabType::EGT_None:
		break;

	}

	if (PhysicsContraintRef) PhysicsContraintRef->BreakConstraint();

	///release the object
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	bIsheld = false;
	if (auto* PrimitiveComp = FindComponentByClass<UStaticMeshComponent>())
		PrimitiveComp->SetSimulatePhysics(true);


	//GrabableType == ELeeXRGrabableType::LeeXROneHand ?
	//	OnRelease(HandBase->GetHandSkeletal()) :
	//	OnReleaseObjects(inComponent);
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

void ALeeXRGrabbableActor::FreeGrababled(UMotionControllerComponent* inMotionController)
{
	if (inMotionController == nullptr || FrezzeOnSnap) return;

	FAttachmentTransformRules AttachRules = GrabType == EGrabType::EGT_Weighted ?
		FAttachmentTransformRules::KeepWorldTransform :
		FAttachmentTransformRules::SnapToTargetNotIncludingScale;

	auto* PrimitiveComp = FindComponentByClass<UStaticMeshComponent>();
	if (PrimitiveComp) {
		PrimitiveComp->SetSimulatePhysics(false);
		if(PrimitiveComp->AttachToComponent(inMotionController, AttachRules, NAME_None))
			MainControllerRef = inMotionController;
	}
}




#endif
