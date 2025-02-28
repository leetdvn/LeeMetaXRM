// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/LeeXRHandTracking.h"
#include "OculusXRHandComponent.h"

ALeeXRHandTracking::ALeeXRHandTracking(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	HandTrackingComp = CreateDefaultSubobject<UOculusXRHandComponent>(TEXT("HandTrackingComp"));
	HandTrackingComp->SetupAttachment(MotionController);


	ControllerType = ELeeXRHandType::LeeXRHandTracking;

	SphereIndex = CreateDefaultSubobject<USphereComponent>(TEXT("SphereIndex"));
	SphereIndex->AttachToComponent(HandTrackingComp,FAttachmentTransformRules::SnapToTargetNotIncludingScale,TEXT("Index3"));

	WidgetInteraction->SetupAttachment(SphereIndex);

	SphereThumb = CreateDefaultSubobject<USphereComponent>(TEXT("SphereThumb"));
	SphereThumb->AttachToComponent(HandTrackingComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Thumb3"));

}


/// <summary>
/// Tracking Grasp ========================================
/// </summary>
void ALeeXRHandTracking::TrackingGrasp()
{
	float dist = FVector::Dist(SphereIndex->GetComponentToWorld().GetLocation(),
		SphereThumb->GetComponentToWorld().GetLocation());

	if (dist >= 4.5f) {
		GraspRelease();
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
}

/// <summary>
/// Grasp Object
void ALeeXRHandTracking::GraspObject()
{
	//Grasp Object Hand Tracking
	CurrentGrabObject = TScriptInterface<ILeeXRInteraction>(HitThumbActor);
	if (CurrentGrabObject) {
		bIsHeld = true;
		FVector GrabLocation = HandTrackingComp->GetComponentLocation();
		CurrentGrabObject->OnGrab(HandTrackingComp, GrabLocation);
	}

	///Timm Distance Count
	if (TimerDelegate.IsBound()) TimerDelegate.Unbind();

	TimerDelegate.BindLambda([this]() {	TrackingGrasp(); });


	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.02f, true);


}

void ALeeXRHandTracking::GraspRelease()
{
	//Grasp Release Hand Tracking
	if (CurrentGrabObject == nullptr)	return;

	CurrentGrabObject->OnRelease(HandTrackingComp);
	CurrentGrabObject = nullptr;
	bIsHeld = false;
}

void ALeeXRHandTracking::BeginPlay()
{
	Super::BeginPlay();

	LEE_SCOPE_CYCLE_COUNTER(ICTUController);

	INC_MEMORY_STAT_BY(STAT_HandController, this->GetResourceSizeBytes(EResourceSizeMode::EstimatedTotal));

	if (SphereIndex->OnComponentHit.IsBound()) SphereIndex->OnComponentHit.Clear();
	SphereIndex->OnComponentHit.AddDynamic(this, &ALeeXRHandTracking::OnComponentIndexHit);

	if (SphereThumb->OnComponentHit.IsBound()) SphereThumb->OnComponentHit.Clear();
	SphereThumb->OnComponentHit.AddDynamic(this, &ALeeXRHandTracking::OnComponentThumbHit);

	if (SphereIndex->OnComponentBeginOverlap.IsBound()) SphereIndex->OnComponentBeginOverlap.Clear();
	SphereIndex->OnComponentBeginOverlap.AddDynamic(this, &ALeeXRHandTracking::OnBeginOverlap);

	if (SphereThumb->OnComponentBeginOverlap.IsBound()) SphereThumb->OnComponentBeginOverlap.Clear();
	SphereThumb->OnComponentBeginOverlap.AddDynamic(this, &ALeeXRHandTracking::OnBeginOverlap);
}

void ALeeXRHandTracking::InittializeSetup()
{
	Super::InittializeSetup();

	SphereIndex->SetSphereRadius(1);
	SphereThumb->SetSphereRadius(1);
}

void ALeeXRHandTracking::OnComponentIndexHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//Do Something
	if (Hit.bBlockingHit) {

		HitIndexActor = Hit.GetActor();
		if (HitThumbActor == HitIndexActor) {
			GraspObject();
		}
	}
	
}

void ALeeXRHandTracking::OnComponentThumbHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Hit.bBlockingHit) {

		HitThumbActor = Hit.GetActor();

		if (HitThumbActor == HitIndexActor)
			GraspObject();
	}
}

void ALeeXRHandTracking::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Do Something
	
	LeeScreenLog("Overlap Comp %s", FColor::Green, *OtherComp->GetName());
}


