// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/LeeXRHandTracking.h"
#include "OculusXRHandComponent.h"
#include "HandPoseRecognizer.h"
#include "Common/LeeXRUltils.h"

using namespace LeeXRUltils;

ALeeXRHandTracking::ALeeXRHandTracking(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	HandTrackingComp = CreateDefaultSubobject<UOculusXRHandComponent>(TEXT("HandTrackingComp"));
	HandTrackingComp->SetupAttachment(MotionController);


	ControllerType = ELeeXRHandType::LeeXRHandTracking;

	SphereIndex = CreateDefaultSubobject<USphereComponent>(TEXT("SphereIndex"));
	SphereIndex->SetupAttachment(HandTrackingComp);

	SphereThumb = CreateDefaultSubobject<USphereComponent>(TEXT("SphereThumb"));
	SphereThumb->SetupAttachment(HandTrackingComp);

	HandPoseRecognizer = CreateDefaultSubobject<UHandPoseRecognizer>(TEXT("HandPoseRecognizer"));
	HandPoseRecognizer->SetupAttachment(HandTrackingComp);
	WidgetInteraction->SetupAttachment(SphereIndex);

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

	TimerDelegate.BindLambda([this]() {TrackingGrasp(); });

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.02f, true);

}

void ALeeXRHandTracking::GraspRelease()
{
	//Grasp Release Hand Tracking
	if (CurrentGrabObject == nullptr)	return;

	CurrentGrabObject->OnRelease(HandTrackingComp);
	CurrentGrabObject = nullptr;
	bIsHeld = false;

	if (TimerHandle.IsValid())
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

}

void ALeeXRHandTracking::BeginPlay()
{
	Super::BeginPlay();

	LEE_SCOPE_CYCLE_COUNTER(ICTUController);

	INC_MEMORY_STAT_BY(STAT_HandController, this->GetResourceSizeBytes(EResourceSizeMode::EstimatedTotal));

	//if (GrabSphere->OnComponentHit.IsBound()) GrabSphere->OnComponentHit.Clear();
	//	GrabSphere->OnComponentHit.AddDynamic(this, &ALeeXRHandTracking::OnComponentHit);
	if (SphereIndex->OnComponentHit.IsBound()) SphereIndex->OnComponentHit.Clear();
	SphereIndex->OnComponentHit.AddDynamic(this, &ALeeXRHandTracking::OnComponentIndexHit);

	if (SphereThumb->OnComponentHit.IsBound()) SphereThumb->OnComponentHit.Clear();
	SphereThumb->OnComponentHit.AddDynamic(this, &ALeeXRHandTracking::OnComponentThumbHit);

	if (SphereIndex->OnComponentBeginOverlap.IsBound()) SphereIndex->OnComponentBeginOverlap.Clear();
	SphereIndex->OnComponentBeginOverlap.AddDynamic(this, &ALeeXRHandTracking::OnBeginOverlap);

	if (SphereThumb->OnComponentBeginOverlap.IsBound()) SphereThumb->OnComponentBeginOverlap.Clear();
	SphereThumb->OnComponentBeginOverlap.AddDynamic(this, &ALeeXRHandTracking::OnBeginOverlap);

	//if (SphereIndex->OnComponentEndOverlap.IsBound()) SphereIndex->OnComponentEndOverlap.Clear();
	//SphereIndex->OnComponentEndOverlap.AddDynamic(this, &ALeeXRHandTracking::OnEndOverlap);

	//if (SphereThumb->OnComponentEndOverlap.IsBound()) SphereThumb->OnComponentEndOverlap.Clear();
	//SphereThumb->OnComponentEndOverlap.AddDynamic(this, &ALeeXRHandTracking::OnEndOverlap);
}

void ALeeXRHandTracking::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HandPoseRecognizer && !HandPoseRecognizer->Poses.IsEmpty()) {

		FString Name;
		int Index;
		float Duration;
		float Error;
		float Confidence;
		HandPoseRecognizer->GetRecognizedHandPose(Index, Name, Duration, Error, Confidence);
		
		LeeHandPose hPose = LeeXRGetEnumValueByString<LeeHandPose>(Name);

		if (Name.EndsWith("None")) return;

		//LeeScreenLog("Hand Pose :%s", FColor::Green, *UEnum::GetValueAsString(hPose));
		switch (hPose)
		{
		case LeeHandPose::LHandMenu:
			//Do Something
			break;
		case LeeHandPose::LHandGrasp:
			//if(!bIsHeld)
				//GraspObject();
			break;
		case LeeHandPose::LHandRelease:
				//GraspRelease();
			break;
		case LeeHandPose::LHandMove:
			//Do Something
			break;
		}

	}
}

void ALeeXRHandTracking::InittializeSetup()
{
	Super::InittializeSetup();


	FTimerHandle AttachHandle;
	GetWorld()->GetTimerManager().SetTimer(AttachHandle, [this]() {
			this->AttachOculusHandTracking(EOculusXRBone::Index_Tip, this->SphereIndex);
			this->AttachOculusHandTracking(EOculusXRBone::Thumb_Tip, this->SphereThumb);
			GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

		},1.f, false, 1.f);

	
	SphereIndex->SetSphereRadius(1.f);
	SphereThumb->SetSphereRadius(1.f);
}

void ALeeXRHandTracking::OnComponentIndexHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//Do Something
	if (Hit.bBlockingHit) {

		//HitIndexActor = Hit.GetActor();
		//if (HitIndexActor == HitThumbActor)
		//	GraspObject();
	}
	
}

void ALeeXRHandTracking::OnComponentThumbHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//Do Something
	if (Hit.bBlockingHit) {
		//HitThumbActor = Hit.GetActor();

		//if (HitThumbActor == HitIndexActor)
		//	GraspObject();
	}
}

void ALeeXRHandTracking::OnBeginOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//LeeScreenLog("Overlap %s", FColor::Green, *OtherActor->GetName());
	HitIndexActor = OtherActor;
	HitThumbActor = OtherActor;
	//if (HitIndexActor == HitThumbActor)
	//	GraspObject();

	LeeScreenLog("1 %s :", FColor::Green, *HitComponent->GetName());

	TArray<AActor*> OverlappingActors;
	OtherComp->GetOverlappingActors(OverlappingActors);

	if (HitIndexActor == HitThumbActor)
		GraspObject();
}


void ALeeXRHandTracking::OnEndOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//LeeScreenLog("End Overlap %s", FColor::Red, *OtherActor->GetName());
}

void ALeeXRHandTracking::AttachOculusHandTracking(const EOculusXRBone inEbone,USphereComponent* inSphere)
{
	FString BoneName = UOculusXRInputFunctionLibrary::GetBoneName(inEbone);
	if (HandTrackingComp->DoesSocketExist(*BoneName)) {
		inSphere->AttachToComponent(HandTrackingComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, *BoneName);
		inSphere->SetSphereRadius(1.f);
		float offset = HandType == EControllerHand::Right ? 1.0f : -1.0f;
		inSphere->SetRelativeLocation(FVector(offset, 0.0f, 0.0f));
	}
}

bool ALeeXRHandTracking::IsGrabable(TArray<AActor*> inOverlapActors, AActor* InArrayAct)
{
	if (inOverlapActors.IsEmpty()) return false;
	
	for (AActor* actor : inOverlapActors) {
		if (actor == InArrayAct) return true;
	}
	return false;
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
	}
}