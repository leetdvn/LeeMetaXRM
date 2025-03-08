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

void ALeeXRGrabbableActor::FreeGrababled(UMotionControllerComponent* inMotionController, bool isWeighted)
{
	if (inMotionController == nullptr || FrezzeOnSnap) return;


	if (isWeighted) {
		PhysicsContraintImplementation(inMotionController);
	}

	FAttachmentTransformRules AttachRules = GrabType == EGrabType::EGT_Weighted ?
		FAttachmentTransformRules::KeepWorldTransform :
		FAttachmentTransformRules::SnapToTargetNotIncludingScale;

	if (auto* PrimitiveComp = FindComponentByClass<UStaticMeshComponent>()) {
		//PrimitiveComp->SetSimulatePhysics(false);
		if (PrimitiveComp->AttachToComponent(inMotionController, AttachRules, NAME_None))
			MainControllerRef = inMotionController;
	}

}

void ALeeXRGrabbableActor::PhysicsContraintImplementation(UMotionControllerComponent* inMCComponent)
{
	LEE_SCOPE_CYCLE_COUNTER(LeeXRGrabable);


	if (auto* HandBase = inMCComponent->GetOwner<ALeeXRHandBase>()) {

		if (auto inPhysicsContraint = HandBase->GetPhysicsConstraint()) {


			//if (PhysicsContraintRef) PhysicsContraintRef->BreakConstraint();

			FString HandSocket = HandBase->GetHandType() == EControllerHand::Left ? "hand_l" : "hand_r";


			if (auto HandSkeletal = HandBase->GetHandSkeletal()) {

				USkeletalMeshComponent* SkeletalComp = FindComponentByClass<USkeletalMeshComponent>();
				UStaticMeshComponent* StaticMeshComp = FindComponentByClass<UStaticMeshComponent>();

				if (StaticMeshComp) {
					inPhysicsContraint->SetConstrainedComponents(HandSkeletalMeshRef, *HandSocket, StaticMeshComp, NAME_None);
					GetWorld()->GetTimerManager().SetTimer(TimerWeighted, [this, StaticMeshComp]() {
						DetachWhenHandThresholdExceed(StaticMeshComp);
						}, 0.02f, true);

				}
				else if (SkeletalComp) {

					inPhysicsContraint->SetConstrainedComponents(HandSkeletalMeshRef, *HandSocket, SkeletalComp, NAME_None);
					GetWorld()->GetTimerManager().SetTimer(TimerWeighted, [this, SkeletalComp]() {
						DetachWhenHandThresholdExceed(SkeletalComp);
						}, 0.02f, true);

				}
				PhysicsContraintRef = inPhysicsContraint;
				HandSkeletalMeshRef = HandSkeletal;
			}

		}
	}

}

//Detaching the Object when the Hand Threshold Exceed
void ALeeXRGrabbableActor::DetachWhenHandThresholdExceed(USkeletalMeshComponent* inSkeletal)
{
	if (inSkeletal == nullptr) return;
	LEE_SCOPE_CYCLE_COUNTER(LeeXRGrabable);

	FVector HMeshLoc = LeeXRGetWorldLocation(inSkeletal);
	if (MainControllerRef) {
		FVector MCloc = LeeXRGetWorldLocation(MainControllerRef);
		float Distance = FVector::Dist(HMeshLoc, MCloc);
		LEE_LOG(LeeXRMacro, Log, "Distance %f", Distance);
		if (Distance > PhysicsGrabThreshold) {
			OnReleaseObjects(MainControllerRef);
		}
	}
}

//Detaching the Object when the Hand Threshold Exceed
void ALeeXRGrabbableActor::DetachWhenHandThresholdExceed(UStaticMeshComponent* inStatiMesh)
{
	if (inStatiMesh == nullptr) return;
	LEE_SCOPE_CYCLE_COUNTER(LeeXRGrabable);

	FVector HMeshLoc = LeeXRGetWorldLocation(inStatiMesh);
	if (MainControllerRef) {
		FVector MCloc = LeeXRGetWorldLocation(MainControllerRef);
		float Distance = FVector::Dist(HMeshLoc, MCloc);
		LEE_LOG(LeeXRMacro, Log, "Distance %f", Distance);
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
			FreeGrababled(inComponent,true);
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



	//if (PhysicsContraintRef) PhysicsContraintRef->BreakConstraint();

	///release the object
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	bIsheld = false;
	if (auto* PrimitiveComp = FindComponentByClass<UStaticMeshComponent>())
		PrimitiveComp->SetSimulatePhysics(true);

	if (auto Skeletal = FindComponentByClass<USkeletalMeshComponent>()) {
		Skeletal->SetSimulatePhysics(true);
	}

	switch (GrabType)
	{
		case EGrabType::EGT_Free: { break; }
		case EGrabType::EGT_Snap:
			break;
		case EGrabType::EGT_Weighted: {
			if (TimerWeighted.IsValid()) {
				UE_LOG(LogTemp, Warning, TEXT("Timer Release"));
				GetWorld()->GetTimerManager().ClearTimer(TimerWeighted);
			}

			break;
		}
		case EGrabType::EGT_None:
			break;

	}

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



#endif
