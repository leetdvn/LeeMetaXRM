// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/LeeXRGrabbableActor.h"
#include <MotionControllerComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <SphereComponent.h>
#include "Actors/LeeXRHandController.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "PhysicsEngine/PhysicsAsset.h"
#include "APawn/LeeXRCharacter.h"


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

void ALeeXRGrabbableActor::SetSmoothGrabableRelease(bool isGrabable, float inAngleDamping, float inLinearDamping)
{
	if (auto* StaticMeshComp = FindComponentByClass<UStaticMeshComponent>()) {
		if (!StaticMeshComp->IsSimulatingPhysics()) return;

		float Linear = isGrabable ? 10.f : inLinearDamping;
		StaticMeshComp->SetLinearDamping(Linear);
	}

}

void ALeeXRGrabbableActor::FreeGrababled(UMotionControllerComponent* inMotionController, bool isWeighted)
{
	if (inMotionController == nullptr || FrezzeOnSnap) return;


	if (isWeighted) {
		PhysicsContraintImplementation(inMotionController);
	}
	else {
		FAttachmentTransformRules AttachRules = GrabType == EGrabType::EGT_Weighted ?
			FAttachmentTransformRules::KeepWorldTransform :
			FAttachmentTransformRules::SnapToTargetNotIncludingScale;

		if (auto* PrimitiveComp = FindComponentByClass<UStaticMeshComponent>()) {
			//PrimitiveComp->SetSimulatePhysics(false);
			if (PrimitiveComp->AttachToComponent(inMotionController, AttachRules, NAME_None))
				MainControllerRef = inMotionController;
		}
	}

}

void ALeeXRGrabbableActor::PhysicsContraintImplementation(UMotionControllerComponent* inMCComponent)
{
	LEE_SCOPE_CYCLE_COUNTER(LeeXRGrabable);

	///Physics Contraint Implementation
	if (auto* HandBase = inMCComponent->GetOwner<ALeeXRHandBase>()) {

		if (auto inPhysicsContraint = HandBase->GetGrabsContraint()) {

			//if (IsValid(GrabledContraintRef)){
			//	if (GrabledContraintRef != inPhysicsContraint) {
			//		GrabledContraintRef->BreakConstraint();
			//	}
			//}

			//GrabledContraintRef = inPhysicsContraint;

			FString HandSocket = HandBase->GetHandType() == EControllerHand::Left ? "hand_l" : "hand_r";
			USkeletalMeshComponent* SkeletalComp = FindComponentByClass<USkeletalMeshComponent>();
			UStaticMeshComponent* StaticMeshComp = FindComponentByClass<UStaticMeshComponent>();

			ALeeXRCharacter* XRCharacter = LeeXRGetCustomCharacter<ALeeXRCharacter>(this);
			bool isLeft = HandBase->GetHandType() == EControllerHand::Left;

			if (auto* skeletal = XRCharacter->GetHandPhysics(isLeft)) {
				//HandSkeletalMeshRef = skeletal;
				XRCharacter->OnPoseMesh.Broadcast(this);

				LEE_LOG(LeeXRMacro, Log, "HandSkeletalMeshRef %s", *skeletal->GetName());
				if (StaticMeshComp) {
					HandBase->PoseableSpawned(RootComponent,XRCharacter->GetHandPhysics(isLeft)->GetSkeletalMeshAsset(), XRCharacter->GetHandPhysics(isLeft));
					inPhysicsContraint->SetConstrainedComponents(skeletal, *HandSocket, StaticMeshComp, NAME_None);
					GetWorld()->GetTimerManager().SetTimer(TimerWeighted, [this, inMCComponent,StaticMeshComp]() {
						DetachWhenHandThresholdExceed(inMCComponent,StaticMeshComp);
						}, 0.01f, true);

				}
				else if (SkeletalComp) {

					inPhysicsContraint->SetConstrainedComponents(skeletal, *HandSocket, SkeletalComp, NAME_None);
					GetWorld()->GetTimerManager().SetTimer(TimerWeighted, [this, inMCComponent, SkeletalComp]() {
						DetachWhenHandThresholdExceed(inMCComponent,SkeletalComp);
						}, 0.01f, true);

				}
				HandSkeletalMeshRef = skeletal;

				GrabledContraintRef = inPhysicsContraint;
				GrabledConstraintsRefs.AddUnique(inPhysicsContraint);

				if (GrabledConstraintsRefs.Num()==2) {
					float MassChanged = .386f * GrabledConstraintsRefs.Num();
					skeletal->SetMassOverrideInKg(NAME_None, MassChanged);
				}

			}

		}
	}

}

//Detaching the Object when the Hand Threshold Exceed
void ALeeXRGrabbableActor::DetachWhenHandThresholdExceed(UMotionControllerComponent* inMCComponent,USkeletalMeshComponent* inSkeletal)
{
	if (inSkeletal == nullptr || inMCComponent == nullptr) return;
	LEE_SCOPE_CYCLE_COUNTER(LeeXRGrabable);


	FVector HMeshLoc = LeeXRGetWorldLocation(inSkeletal);
	FVector MCloc = LeeXRGetWorldLocation(inMCComponent);
	float Distance = FVector::Dist(HMeshLoc, MCloc);
	LEE_LOG(LeeXRMacro, Log, "Distance %f", Distance);
	if (Distance > PhysicsGrabThreshold) {
		OnReleaseObjects(inMCComponent);

	}
}

//Detaching the Object when the Hand Threshold Exceed
void ALeeXRGrabbableActor::DetachWhenHandThresholdExceed(UMotionControllerComponent* inMCComponent,UStaticMeshComponent* inStatiMesh)
{
	if (inStatiMesh == nullptr || inMCComponent==nullptr) return;
	LEE_SCOPE_CYCLE_COUNTER(LeeXRGrabable);

	FVector HMeshLoc = LeeXRGetWorldLocation(inStatiMesh);
	FVector MCloc = LeeXRGetWorldLocation(inMCComponent);
	float Distance = FVector::Dist(HMeshLoc, MCloc);
	LEE_LOG(LeeXRMacro, Log, "Distance %f", Distance);
	if (Distance > PhysicsGrabThreshold) {
		OnReleaseObjects(inMCComponent);
	}
}

// Called when the game starts or when spawned
void ALeeXRGrabbableActor::BeginPlay()
{
	Super::BeginPlay();	


	LEE_SCOPE_CYCLE_COUNTER(LeeXRGrabable);

	ADDMEMORYSTAT(this, STAT_LeeXRGrabableMemory);
	if (UStaticMeshComponent* StaticMeshComp = FindComponentByClass<UStaticMeshComponent>()) {
		StaticMeshComp->SetMassOverrideInKg(NAME_None, LeeMassDefault);
	}
	//Settitng Init When Game Starts
	InitSettings();
}

void ALeeXRGrabbableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (GrabledConstraintsRefs.IsEmpty()) return;

	//if (GrabledConstraintsRefs.IsValidIndex(2))
	//{
	//	if (GrabledConstraintsRefs.Num() == 2)
	//	{
	//		LeeScreenLog("Two Hand Constraint",FColor::Purple);
	//	}
	//}

}

void ALeeXRGrabbableActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	SET_MEMORY_STAT(STAT_LeeXRGrabableMemory, 0);
	//MemoriesSize = 0;
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
	
	switch (GrabType)
	{
		case EGrabType::EGT_Free: {
			FreeGrababled(inComponent);
		}
		case EGrabType::EGT_Snap: {
			break;
		}
		case EGrabType::EGT_Weighted: {
			PhysicsContraintImplementation(inComponent);
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

	if(bIsheld){

		
		switch (GrabType)
		{
			case EGrabType::EGT_Free: {
				DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				bIsheld = false;
				if (auto* PrimitiveComp = FindComponentByClass<UStaticMeshComponent>())
					PrimitiveComp->SetSimulatePhysics(true);

				if (auto Skeletal = FindComponentByClass<USkeletalMeshComponent>()) {
					Skeletal->SetSimulatePhysics(true);
				}
				break;
			}

			case EGrabType::EGT_Snap:
				break;
			case EGrabType::EGT_Weighted: {
				if (TimerWeighted.IsValid()) {
					//UE_LOG(LogTemp, Warning, TEXT("Timer Release"));
					GetWorld()->GetTimerManager().ClearTimer(TimerWeighted);
					if (auto* HandBase = inComponent->GetOwner<ALeeXRHandBase>()) {
						if (auto inPhysicsContraint = HandBase->GetGrabsContraint()) {
							inPhysicsContraint->BreakConstraint();
							GrabledConstraintsRefs.Remove(inPhysicsContraint);
							HandBase->PoseableDestroyed();
						}
					}
			
				}

				break;
			}
			case EGrabType::EGT_None:
				break;

		}

	}
	else {
		if (TimerWeighted.IsValid())
			GetWorld()->GetTimerManager().ClearTimer(TimerWeighted);

		if (auto* HandBase = inComponent->GetOwner<ALeeXRHandBase>()) {
			if (auto inPhysicsContraint = HandBase->GetGrabsContraint()) {
				inPhysicsContraint->BreakConstraint();
				GrabledConstraintsRefs.Remove(inPhysicsContraint);
				HandBase->PoseableDestroyed();
			}
		}
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
